#include "TLSFMemoryAllocator.h"

#include <array>

namespace {
	const std::array<size_t,32> _eo2_table = {
		1u << 0, 1u << 1, 1u << 3, 1u << 4, 1u << 5, 1u << 6, 1u << 7, 1u << 8, 1u << 9,
		1u << 10, 1u << 11, 1u << 13, 1u << 14, 1u << 15, 1u << 16, 1u << 17, 1u << 18, 1u << 19,
		1u << 20, 1u << 21, 1u << 23, 1u << 24, 1u << 25, 1u << 26, 1u << 27, 1u << 28, 1u << 29,
		1u << 30, 1u << 31
	};
	const size_t SECOND_LEVEL_INDEX_SEPARATION_EXPONENT_OF_TWO(5); //第二カテゴリ分割数べき乗数(2の冪乗数でないといけない2^4=16or2^5=32が適切らしい。)

	//http://sackys-blog-extend.cocolog-nifty.com/blog/2012/07/tlsf-5-e929.html
	inline static unsigned _LSB(unsigned int word)
	{
		int bit = 0;

		if (word)
		{
			word &= ~word + 1;
			if (word & 0xffff0000) bit += 16;
			if (word & 0xff00ff00) bit += 8;
			if (word & 0xf0f0f0f0) bit += 4;
			if (word & 0xcccccccc) bit += 2;
			if (word & 0xaaaaaaaa) ++bit;
			++bit;
		}
		return bit;
	}
	inline static unsigned _MSB(unsigned int word)
	{
		int bit = 32;

		if (!word) --bit;
		if (!(word & 0xffff0000)) { word <<= 16; bit -= 16; }
		if (!(word & 0xff000000)) { word <<= 8; bit -= 8; }
		if (!(word & 0xf0000000)) { word <<= 4; bit -= 4; }
		if (!(word & 0xc0000000)) { word <<= 2; bit -= 2; }
		if (!(word & 0x80000000)) { word <<= 1; --bit; }

		return bit;
	}

	//http://marupeke296.com/cgi-bin/cbbs/cbbs.cgi?mode=al2&namber=5198&rev=&no=0&P=R&KLOG=5
	static unsigned count1_32(unsigned val) {
		val = (val & 0x55555555) + ((val >> 1) & 0x55555555);
		val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
		val = (val & 0x0f0f0f0f) + ((val >> 4) & 0x0f0f0f0f);
		val = (val & 0x00ff00ff) + ((val >> 8) & 0x00ff00ff);
		return  (val & 0x0000ffff) + ((val >> 16) & 0x0000ffff);
	}
	static bool LSB32(unsigned val, unsigned* out) {
		if (val == 0) return false;
		val |= (val << 1);
		val |= (val << 2);
		val |= (val << 4);
		val |= (val << 8);
		val |= (val << 16);
		*out = 32 - count1_32(val);
		return true;
	}
	static bool MSB32(unsigned val, unsigned* out) {
		if (val == 0) return false;
		val |= (val >> 1);
		val |= (val >> 2);
		val |= (val >> 4);
		val |= (val >> 8);
		val |= (val >> 16);
		*out = count1_32(val) - 1;
		return true;
	}
	inline unsigned long getMSB(unsigned size) {
//		return _MSB(size) - 1;
		unsigned long index;
		_BitScanReverse(&index, size);
		return index;
	}
	inline unsigned long getLSB(unsigned size) {
//		return _LSB(size) - 1;
		unsigned long index;
		_BitScanForward(&index, size);
		return index;
	}
	int getFreeListSLI(unsigned int mySLI, unsigned freeListBit) {
		// 自分のSLI以上が立っているビット列を作成 (ID = 0なら0xffffffff）
		unsigned myBit = 0xffffffff << mySLI;

		// myBitとfreeListBitを論理積すれば、確保可能なブロックを持つSLIが出てきます
		//  freeListBit = 1100 ([192-223]と[224-255]にあるとします）
		unsigned enableListBit = freeListBit & myBit;

		// LSBを求めれば確保可能な一番サイズの小さいフリーリストブロックの発見
		if (enableListBit == 0)
			return -1;   // フリーリスト無し
		return static_cast<int>(getLSB(enableListBit));
	}
}

namespace planeta_engine {
	namespace core {
		//////////////////////////////////////////////////////////////////////////
		//TLSFTLSFMemoryAllocator::_Impl
		/////////////////////////////////////////////////////////////////////////
		class TLSFMemoryAllocator::_Impl {
		public:
			_Impl(TLSFMemoryAllocator& ptlsf) :_tlsf(ptlsf), _second_level_index_separation_exponent_of_2(SECOND_LEVEL_INDEX_SEPARATION_EXPONENT_OF_TWO), _second_level_index_separation(1u << _second_level_index_separation_exponent_of_2), _free_block_list(nullptr), _free_block_list_size(0) {}
			~_Impl() { release_free_block_list(); }
			bool reserve_memory(size_t size);
			MemoryDetailScanResult detail_scan()const;
			void* alloc(size_t size);
			void dealloc(void*);
			void release_free_block_list(); //フリーブロックリストように確保していた領域を解放
		private:
			class _MemoryBlock {
			public:
				_MemoryBlock(); //ダミー作成ようコンストラクタ
				_MemoryBlock(bool footer_valid_flag); //コンストラクタ(trueでフッター書き込み。falseで書き込みなし)
				_MemoryBlock(size_t data_size); //指定データサイズのメモリブロック作成
				_MemoryBlock(size_t size, bool fit); //第二引数がtrueなら、合計サイズが第一引数になるように、falseなら第一引数がデータサイズになるようにメモリブロックを作成する
				~_MemoryBlock();
				static void* operator new(size_t size, void* buf){ return buf; } //placement new
					static void operator delete(void* p, void* buf) {} //placement delete(使わない)
				static void* operator new(size_t size){ return malloc(size); } //通常のnew
				static void operator delete(void* p) { free(p); } //通常のdelete
				static void* operator new[](size_t size) { return malloc(size); } //通常のnew[]
				static void operator delete[](void* p) { free(p); } //通常のdelete[]
				size_t GetDataSize()const { return _data_size; }
				size_t GetTotalSize()const { return CalcTotalSizeByDataSize(_data_size); }
				char* GetDataBlockPointer() { return reinterpret_cast<char*>(this) + header_size; }
				size_t* GetFooterPointer() { return reinterpret_cast<size_t*>(reinterpret_cast<char*>(GetDataBlockPointer()) + _data_size); }
				bool ErrorCheck()const { return _error_check_data != error_check_code; } //エラーチェック。エラーならtrue
				bool FooterErrorCheck()const { return GetTotalSize() != *const_cast<_MemoryBlock*>(this)->GetFooterPointer(); } //フッターの値が正当化確認

				void LeaveFromList(); //現在のリストから外れる
				void JoinToLIst(_MemoryBlock*); //指定ブロックの後ろに追加

				bool IsUsing()const { return _use_flag; }
				void SetUsingFlag(bool f) { _use_flag = f; }
				static size_t CalcTotalSizeByDataSize(size_t data_size) { return data_size + manage_area_size; } //データサイズから、ブロックの号掲載ぞを計算する

				_MemoryBlock* next() { return _next; }
				_MemoryBlock* prev() { return _prev; }

				const static size_t header_size; //ヘッダサイズ
				const static size_t footer_size; //フッタサイズ
				const static size_t manage_area_size; //管理用領域サイズ(ヘッダ+フッタ)
				const static size_t error_check_code; //エラー確認コード
			private:
				bool _use_flag; //使用中フラグ
				size_t _data_size; //データ領域のサイズ
				_MemoryBlock *_prev; //リストの前へのポインタ
				_MemoryBlock *_next; //リストの後ろへのポインタ
				size_t _error_check_data; //エラー確認用データ
			};
			_MemoryBlock* _free_block_list; //フリーブロックリスト
			size_t _free_block_list_size;
			std::vector<size_t> _second_level_index_free_bit_array; //フリーブロックリスト第二カテゴリビット
			size_t _first_level_index_free_bit_array; //フリーブロックリスト第一カテゴリビット

			void _init_free_block_list(size_t cate1_size, size_t cate2_size); //フリーブロック初期化
			void _push_free_block(_MemoryBlock* block); //フリーブロック追加
			_MemoryBlock* _pop_free_block(size_t cate1, size_t cate2); //フリーブロックから指定されたカテゴリが格納可能なブロックを返す
			void _remove_free_block(_MemoryBlock* block);
			_MemoryBlock* _marge_free_block(_MemoryBlock*); //可能ならフローブロックを結合する(この関数内ではフリーブロックリストへの登録は行わない。マージしたブロックの登録解除は行う)
			_MemoryBlock* _devide_free_block(_MemoryBlock*, size_t); //可能ならフリーブロックを指定サイズに分割する。分割した右ブロックへのポインタを返す(この関数内ではフリーブロックリストへの登録は行わない)

			size_t _second_level_index_separation_exponent_of_2; //第二カテゴリの分割数の2べき乗
			size_t _second_level_index_separation; //第二カテゴリの分割数

			//最上位ビットインデックスを求める
			static size_t calcMSB(size_t size); //最上位ビットを求める
//				static size_t calcLSB(size_t size);
			size_t _calc_second_level_index(size_t size, size_t first_category); //第二カテゴリを計算する
			size_t _get_free_block_list_index(size_t cate1, size_t cate2); //フリーブロックリストのインデックスを計算する。

			TLSFMemoryAllocator& _tlsf;
		};

		inline bool planeta_engine::core::TLSFMemoryAllocator::_Impl::reserve_memory(size_t size)
		{
			if (_tlsf._memory_pool) { free(_tlsf._memory_pool); } //確保してある領域を解放
			_tlsf._allocated_memory_count = 0;
			_tlsf._allcated_memory_size = 0;
			_tlsf._using_memory_size = 0;
			//最低管理領域(両端のダミーブロック、一つ目のデータブロックの合計領域と最低データ領域(第二カテゴリ分割数))に足りなかったらエラー
			if (size <= _MemoryBlock::manage_area_size * 3 + _second_level_index_separation) { return false; }
			_tlsf._memory_pool = reinterpret_cast<char*>(std::malloc(size)); //メモリ確保
			_tlsf._memory_pool_size = size;
			//前後にダミーブロックを作成
			_MemoryBlock* head_dumy = new(_tlsf._memory_pool)_MemoryBlock();
			_MemoryBlock* foot_dumy = new(reinterpret_cast<char*>(_tlsf._memory_pool) + _tlsf._memory_pool_size - _MemoryBlock::manage_area_size)_MemoryBlock();
			head_dumy->SetUsingFlag(true);
			foot_dumy->SetUsingFlag(true);
			_tlsf._using_memory_size += head_dumy->GetTotalSize();
			_tlsf._using_memory_size += foot_dumy->GetTotalSize();
			//領域に収まるようにメモリブロック作成
			size_t create_block_size = (_tlsf._memory_pool_size - head_dumy->GetTotalSize() - foot_dumy->GetTotalSize());
			_MemoryBlock* block = new(reinterpret_cast<char*>(head_dumy) + head_dumy->GetTotalSize())_MemoryBlock(create_block_size, true);
			//作製したデータの格納カテゴリを求める
			size_t cate1 = calcMSB(block->GetDataSize() + 1);
			size_t cate2 = _calc_second_level_index(block->GetDataSize() + 1, cate1);
			if (cate2 == 0) { --cate1; cate2 = _second_level_index_separation - 1; }
			else { --cate2; }
			//指定されたサイズ分のフリーブロックリスト領域を確保
			_init_free_block_list(cate1 + 1, cate2 + 1);
			//確保したメモリブロックをフリーブロックリストに登録
			_push_free_block(block);
			return true;
		}

		inline TLSFMemoryAllocator::MemoryDetailScanResult TLSFMemoryAllocator::_Impl::detail_scan() const
		{
			MemoryDetailScanResult res;
			if (_tlsf._memory_pool == nullptr) { return res; }
			char* ptr = _tlsf._memory_pool;
			//メモリブロック探索
			while (ptr < _tlsf._memory_pool + _tlsf._memory_pool_size) {
				_MemoryBlock* block = reinterpret_cast<_MemoryBlock*>(ptr);
				if (block->ErrorCheck() || block->FooterErrorCheck()) { res.corruption = true; break; } //エラーチェック
				size_t data_size = block->GetDataSize();
				size_t total_size = block->GetTotalSize();
				bool use_flag = block->IsUsing();
				if (use_flag) { //アクティブブロック
					if (data_size > 0) { ++res.active_block_count; }
					else { ++res.dumy_block_count; }
					res.allocated_memory_size += data_size;
					res.using_memory_size += total_size;
				}
				else { //フリーブロック
					if (data_size > 0) { ++res.free_block_count; }
					else { ++res.dumy_block_count; }
					res.available_memory_size += data_size;
					res.free_memory_size += total_size;
					if (res.available_max_memory_allocation_size < data_size) { res.available_max_memory_allocation_size = data_size; }
				}
				++res.total_block_count;
				res.reserved_memory_size += total_size;
				ptr += total_size;
			}
			//フリーブロックリストのスキャン
			size_t free_list_count = 0;
			size_t correct_first_free_block_bit_array = 0;
			for (size_t i = 0; i < _free_block_list_size; ++i) {
				bool is_exist_free_block = false;
				_MemoryBlock* list_top = &_free_block_list[i];
				while (list_top == list_top->next()) { ++free_list_count; is_exist_free_block = true; }
				if ((((_second_level_index_free_bit_array[i / _second_level_index_separation])&(1u << (i % _second_level_index_separation))) ? true : false) != (is_exist_free_block ? true : false)) {
					res.inconsistency_at_second_free_block_list_bit_array = true;
				}
				if (is_exist_free_block) { correct_first_free_block_bit_array |= (1u << (i / _second_level_index_separation)); }
			}
			//エラー検出
			res.inconsistency_at_allocated_memory_count = (_tlsf._allocated_memory_count != res.active_block_count);
			res.inconsistency_at_allocated_memory_size = (_tlsf._allcated_memory_size != res.allocated_memory_size);
			res.inconsistency_at_reserved_memory_size = (res.reserved_memory_size != _tlsf._memory_pool_size);
			res.inconsistency_at_using_memory_size = (res.using_memory_size != _tlsf._using_memory_size);
			res.inconsistency_at_dumy_count = (res.dumy_block_count != 2);
			res.inconsistency_at_free_block_list = res.free_block_count != free_list_count;
			res.inconsistency_at_second_free_block_list_bit_array = (_first_level_index_free_bit_array != correct_first_free_block_bit_array);
			res.error = res.corruption || res.inconsistency_at_allocated_memory_count || res.inconsistency_at_allocated_memory_size || res.inconsistency_at_using_memory_size || res.inconsistency_at_dumy_count || res.inconsistency_at_free_block_list || res.inconsistency_at_first_free_block_list_bit_array || res.inconsistency_at_second_free_block_list_bit_array;
			//断片化率計算
			res.fragment_ratio = 1.0 - ((double)res.available_max_memory_allocation_size / res.available_memory_size);
			return res;
		}

		inline void* TLSFMemoryAllocator::_Impl::alloc(size_t size)
		{
			if (size == 0) { throw BadAlloc("Size must not equal zero."); }
			//要求メモリのカテゴリを確認
			size_t cate_1 = calcMSB(size);
			size_t cate_2 = _calc_second_level_index(size, cate_1);
			_MemoryBlock* _allocated_block = _pop_free_block(cate_1, cate_2);
			if (_allocated_block == nullptr) {
				char err[256] = "\0";
				sprintf_s(err, 256, "failed to allocate memory. avalable free block is not exist. (size : %ul)", size);
				throw BadAlloc(err);
			}
			//可能なら分割する
			_MemoryBlock* remain_block = _devide_free_block(_allocated_block, size);
			_allocated_block->SetUsingFlag(true);
			if (remain_block) { _push_free_block(remain_block); }

			_tlsf._allcated_memory_size += _allocated_block->GetDataSize();
			_tlsf._using_memory_size += _allocated_block->GetTotalSize();
			++_tlsf._allocated_memory_count;

			return _allocated_block->GetDataBlockPointer(); //データ領域のポインタを返す
		}

		inline void TLSFMemoryAllocator::_Impl::dealloc(void* ptr)
		{
			//指定ポインタが確保領域内か確認(両端のダミー領域も範囲外とみなす)
			if (ptr < (_tlsf._memory_pool + _MemoryBlock::manage_area_size) || ptr >= (_tlsf._memory_pool + _tlsf._memory_pool_size - _MemoryBlock::manage_area_size)) { throw BadAlloc("Designated pointer is out of range,"); }
			_MemoryBlock* _block = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(ptr) - sizeof(_MemoryBlock));
			if (_block->ErrorCheck()) { throw BadAlloc("Designated pointer is invalied."); } //指定ポインターが有効か確認
			if (!_block->IsUsing()) { throw BadAlloc("Designated pointer is already deallocated."); } //指定ポインターが使用中か確認
			_tlsf._allcated_memory_size -= _block->GetDataSize();
			_tlsf._using_memory_size -= _block->GetTotalSize();
			--_tlsf._allocated_memory_count;

			_block->SetUsingFlag(false);
			//必要ならマージする
			_block = _marge_free_block(_block);
			//フリーリストに追加
			_push_free_block(_block);
		}

		inline void TLSFMemoryAllocator::_Impl::release_free_block_list() {
			if (_free_block_list) { /*delete[] _free_block_list;*/free(_free_block_list); }
			_free_block_list_size = 0;
		}

		inline void TLSFMemoryAllocator::_Impl::_init_free_block_list(size_t cate1_size, size_t cate2_size)
		{
			release_free_block_list();
			size_t fbl_size = (cate1_size - 1) * _second_level_index_separation + cate2_size;
			_free_block_list = reinterpret_cast<_MemoryBlock*>(malloc(sizeof(_MemoryBlock)*fbl_size)); //領域確保して
			for (unsigned int i = 0; i < fbl_size; ++i) {
				new(&_free_block_list[i]) _MemoryBlock(false); //配置newで初期化
			}
			_free_block_list_size = fbl_size;
			_first_level_index_free_bit_array = 0;
			_second_level_index_free_bit_array.clear();
			_second_level_index_free_bit_array.resize(cate1_size, 0);
		}

		inline void TLSFMemoryAllocator::_Impl::_push_free_block(_MemoryBlock* block) {
			//自分の所属カテゴリより一つ下にいれる
			size_t cate1 = calcMSB(block->GetDataSize() + 1);
			size_t cate2 = _calc_second_level_index(block->GetDataSize() + 1, cate1);
			if (cate2 == 0) { --cate1; cate2 = _second_level_index_separation - 1; }
			else { --cate2; }
			block->JoinToLIst(&_free_block_list[_get_free_block_list_index(cate1, cate2)]);
			_first_level_index_free_bit_array |= (1u << cate1);
			_second_level_index_free_bit_array[cate1] |= (1u << cate2);
		}

		//////////////////////////////////////////////////////////////////////////
		//TLSFTLSFMemoryAllocator::_Impl::_MemoryBlock
		/////////////////////////////////////////////////////////////////////////
		inline TLSFMemoryAllocator::_Impl::_MemoryBlock* TLSFMemoryAllocator::_Impl::_pop_free_block(size_t cate_1, size_t cate_2) {
			int cate_1u((signed)cate_1), cate_2u((signed)cate_2);
			cate_2u = getFreeListSLI(cate_2, _second_level_index_free_bit_array[cate_1]);
			if (cate_2u < 0) { //なかったら所属可能な第一カテゴリを探す
				cate_1u = getFreeListSLI(cate_1, _first_level_index_free_bit_array);
				if (cate_1u >= 0) {
					cate_2u = getFreeListSLI(0, _second_level_index_free_bit_array[cate_1u]);
					if (cate_2u < 0) { throw std::logic_error("フリーブロックリストビットの矛盾"); } //割り当て失敗(ありえない)
				}
				else { return nullptr; } //割り当て失敗
			}
			cate_1 = (unsigned)cate_1u;
			cate_2 = (unsigned)cate_2u;
			size_t free_list_idx = _get_free_block_list_index(cate_1, cate_2);
			if (free_list_idx >= _free_block_list_size) { throw std::logic_error("attempt to access out of free bit list! "); }
			_MemoryBlock* _allocated_block = _free_block_list[free_list_idx].next();
			_remove_free_block(_allocated_block); //フリーブロックリストから除去
			return _allocated_block;
		}

		inline void TLSFMemoryAllocator::_Impl::_remove_free_block(_MemoryBlock* block) {
			block->LeaveFromList();
			size_t cate1 = calcMSB(block->GetDataSize() + 1);
			size_t cate2 = _calc_second_level_index(block->GetDataSize() + 1, cate1);
			if (cate2 == 0) { --cate1; cate2 = _second_level_index_separation - 1; }
			else { --cate2; }
			size_t idx = _get_free_block_list_index(cate1, cate2);
			if (_free_block_list[idx].next() == nullptr) {
				_second_level_index_free_bit_array[cate1] &= ~(1u << cate2);
				if (_second_level_index_free_bit_array[cate1] == 0) { _first_level_index_free_bit_array &= ~(1u << cate1); }
			}
		}

		inline size_t TLSFMemoryAllocator::_Impl::calcMSB(size_t size)
		{
			return getMSB(size);
			/*size_t ret = 16;
			unsigned int seg = 16;
			while ((seg >>= 1)) {
				size_t e2 = _eo2_table[ret - 1];
				if (size > e2) { ret += seg; }
				else if (size < e2) { ret -= seg; }
				else { break; }
			}
			if (_eo2_table[ret - 1] > size) { --ret; }
			return ret;*/
		}

		inline size_t TLSFMemoryAllocator::_Impl::_calc_second_level_index(size_t size, size_t first_category)
		{
			// 最上位ビット未満のビット列だけを有効にするマスク
			const unsigned mask = (1 << first_category) - 1;   // 1000 0000 -> 0111 1111
			// 右へのシフト数を算出
			const unsigned rs = first_category - _second_level_index_separation_exponent_of_2;    // 7 - 3 = 4 （8分割ならN=3です）
			// 引数sizeにマスクをかけて、右へシフトすればインデックスに
			return (size & mask) >> rs;
		}

		inline size_t TLSFMemoryAllocator::_Impl::_get_free_block_list_index(size_t cate1, size_t cate2)
		{
			return cate1 * (1u << _second_level_index_separation_exponent_of_2) + cate2 - 1;
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock* TLSFMemoryAllocator::_Impl::_marge_free_block(_MemoryBlock* block) {
			//右
			_MemoryBlock* right = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(block) + block->GetTotalSize());
			if (!right->IsUsing()) {
				_remove_free_block(right);
				new(block)_MemoryBlock(block->GetTotalSize() + right->GetDataSize());
			}
			//左
			size_t tsize = *(reinterpret_cast<size_t*>(reinterpret_cast<char*>(block) - _MemoryBlock::footer_size));
			_MemoryBlock* left = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(block) - tsize);
			if (!left->IsUsing()) {
				_remove_free_block(left);
				block = new(left)_MemoryBlock(left->GetTotalSize() + block->GetDataSize());
			}
			return block;
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock* TLSFMemoryAllocator::_Impl::_devide_free_block(_MemoryBlock* block, size_t size) {
			//分割後の残り領域サイズが最低値より小さかったら分割しない確保できないなら
			if (block->GetDataSize() <= size + sizeof(_MemoryBlock) + _MemoryBlock::footer_size + _second_level_index_separation) { return nullptr; }
			size_t remain_data_size = block->GetDataSize() - size - sizeof(_MemoryBlock) - _MemoryBlock::footer_size;
			new(block)_MemoryBlock(size);
			block = new(reinterpret_cast<char*>(block) + block->GetTotalSize())_MemoryBlock(remain_data_size);
			return block;
		}

		const size_t TLSFMemoryAllocator::_Impl::_MemoryBlock::footer_size = sizeof(size_t);
		const size_t TLSFMemoryAllocator::_Impl::_MemoryBlock::header_size = sizeof(_MemoryBlock);
		const size_t TLSFMemoryAllocator::_Impl::_MemoryBlock::manage_area_size = (footer_size + header_size);
		const size_t TLSFMemoryAllocator::_Impl::_MemoryBlock::error_check_code = 0xCDECECEC;

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock::_MemoryBlock(size_t data_size) : _data_size(data_size), _use_flag(false), _prev(nullptr), _next(nullptr), _error_check_data(error_check_code)
		{
			*GetFooterPointer() = GetTotalSize();
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock::_MemoryBlock(size_t size, bool fit) : _MemoryBlock(fit ? size - sizeof(_MemoryBlock) - footer_size : size)
		{
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock::_MemoryBlock(bool list_top_flag) : _data_size(0), _use_flag(false), _prev(nullptr), _next(nullptr), _error_check_data(error_check_code) {
			//フッターに合計サイズ書き込み
			if (list_top_flag) { *GetFooterPointer() = GetTotalSize(); }
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock::_MemoryBlock() : _MemoryBlock((size_t)0)
		{
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock::~_MemoryBlock()
		{
		}

		inline void TLSFMemoryAllocator::_Impl::_MemoryBlock::LeaveFromList() {
			if (_prev) { _prev->_next = _next; }
			if (_next) { _next->_prev = _prev; }
			_prev = nullptr;
			_next = nullptr;
		}

		inline void TLSFMemoryAllocator::_Impl::_MemoryBlock::JoinToLIst(_MemoryBlock* _pre_block) {
			_prev = _pre_block;
			_next = _prev->_next;
			_pre_block->_next = this;
			if (_next) { _next->_prev = this; }
		}

		//////////////////////////////////////////////////////////////////////////
		//TLSFTLSFMemoryAllocator
		/////////////////////////////////////////////////////////////////////////
		TLSFMemoryAllocator::TLSFMemoryAllocator() : _memory_pool_size(0), _memory_pool(nullptr), _using_memory_size(0), _allocated_memory_count(0), _impl(new _Impl(*this))
		{
		}

		TLSFMemoryAllocator::TLSFMemoryAllocator(size_t t) :TLSFMemoryAllocator(){
			ReserveMemory(t);
		}

		TLSFMemoryAllocator::~TLSFMemoryAllocator()
		{
			if (_memory_pool) { free(_memory_pool); }
			delete _impl;
		}

		bool TLSFMemoryAllocator::ReserveMemory(size_t size) { return _impl->reserve_memory(size); }

		void TLSFMemoryAllocator::ReleaseMemory() {
			if (_memory_pool) { free(_memory_pool); }
			_impl->release_free_block_list();
		}

		TLSFMemoryAllocator::MemoryDetailScanResult TLSFMemoryAllocator::DetailScan() const { return _impl->detail_scan(); }

		void* TLSFMemoryAllocator::_alloc(size_t size) { return _impl->alloc(size); }

		void TLSFMemoryAllocator::_dealloc(void* ptr) { return _impl->dealloc(ptr); }
	}
}
