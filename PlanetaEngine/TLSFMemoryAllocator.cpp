#include "TLSFMemoryAllocator.h"

#include <array>

namespace {
	const std::array<size_t,32> _eo2_table = {
		1u << 0, 1u << 1, 1u << 3, 1u << 4, 1u << 5, 1u << 6, 1u << 7, 1u << 8, 1u << 9,
		1u << 10, 1u << 11, 1u << 13, 1u << 14, 1u << 15, 1u << 16, 1u << 17, 1u << 18, 1u << 19,
		1u << 20, 1u << 21, 1u << 23, 1u << 24, 1u << 25, 1u << 26, 1u << 27, 1u << 28, 1u << 29,
		1u << 30, 1u << 31
	};
	const size_t SECOND_LEVEL_INDEX_SEPARATION_EXPONENT_OF_TWO(5); //���J�e�S���������ׂ��搔(2�̙p�搔�łȂ��Ƃ����Ȃ�2^4=16or2^5=32���K�؂炵���B)

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
		// ������SLI�ȏオ�����Ă���r�b�g����쐬 (ID = 0�Ȃ�0xffffffff�j
		unsigned myBit = 0xffffffff << mySLI;

		// myBit��freeListBit��_���ς���΁A�m�ۉ\�ȃu���b�N������SLI���o�Ă��܂�
		//  freeListBit = 1100 ([192-223]��[224-255]�ɂ���Ƃ��܂��j
		unsigned enableListBit = freeListBit & myBit;

		// LSB�����߂�Ίm�ۉ\�Ȉ�ԃT�C�Y�̏������t���[���X�g�u���b�N�̔���
		if (enableListBit == 0)
			return -1;   // �t���[���X�g����
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
			void release_free_block_list(); //�t���[�u���b�N���X�g�悤�Ɋm�ۂ��Ă����̈�����
		private:
			class _MemoryBlock {
			public:
				_MemoryBlock(); //�_�~�[�쐬�悤�R���X�g���N�^
				_MemoryBlock(bool footer_valid_flag); //�R���X�g���N�^(true�Ńt�b�^�[�������݁Bfalse�ŏ������݂Ȃ�)
				_MemoryBlock(size_t data_size); //�w��f�[�^�T�C�Y�̃������u���b�N�쐬
				_MemoryBlock(size_t size, bool fit); //��������true�Ȃ�A���v�T�C�Y���������ɂȂ�悤�ɁAfalse�Ȃ���������f�[�^�T�C�Y�ɂȂ�悤�Ƀ������u���b�N���쐬����
				~_MemoryBlock();
				static void* operator new(size_t size, void* buf){ return buf; } //placement new
					static void operator delete(void* p, void* buf) {} //placement delete(�g��Ȃ�)
				static void* operator new(size_t size){ return malloc(size); } //�ʏ��new
				static void operator delete(void* p) { free(p); } //�ʏ��delete
				static void* operator new[](size_t size) { return malloc(size); } //�ʏ��new[]
				static void operator delete[](void* p) { free(p); } //�ʏ��delete[]
				size_t GetDataSize()const { return _data_size; }
				size_t GetTotalSize()const { return CalcTotalSizeByDataSize(_data_size); }
				char* GetDataBlockPointer() { return reinterpret_cast<char*>(this) + header_size; }
				size_t* GetFooterPointer() { return reinterpret_cast<size_t*>(reinterpret_cast<char*>(GetDataBlockPointer()) + _data_size); }
				bool ErrorCheck()const { return _error_check_data != error_check_code; } //�G���[�`�F�b�N�B�G���[�Ȃ�true
				bool FooterErrorCheck()const { return GetTotalSize() != *const_cast<_MemoryBlock*>(this)->GetFooterPointer(); } //�t�b�^�[�̒l���������m�F

				void LeaveFromList(); //���݂̃��X�g����O���
				void JoinToLIst(_MemoryBlock*); //�w��u���b�N�̌��ɒǉ�

				bool IsUsing()const { return _use_flag; }
				void SetUsingFlag(bool f) { _use_flag = f; }
				static size_t CalcTotalSizeByDataSize(size_t data_size) { return data_size + manage_area_size; } //�f�[�^�T�C�Y����A�u���b�N�̍��f�ڂ����v�Z����

				_MemoryBlock* next() { return _next; }
				_MemoryBlock* prev() { return _prev; }

				const static size_t header_size; //�w�b�_�T�C�Y
				const static size_t footer_size; //�t�b�^�T�C�Y
				const static size_t manage_area_size; //�Ǘ��p�̈�T�C�Y(�w�b�_+�t�b�^)
				const static size_t error_check_code; //�G���[�m�F�R�[�h
			private:
				bool _use_flag; //�g�p���t���O
				size_t _data_size; //�f�[�^�̈�̃T�C�Y
				_MemoryBlock *_prev; //���X�g�̑O�ւ̃|�C���^
				_MemoryBlock *_next; //���X�g�̌��ւ̃|�C���^
				size_t _error_check_data; //�G���[�m�F�p�f�[�^
			};
			_MemoryBlock* _free_block_list; //�t���[�u���b�N���X�g
			size_t _free_block_list_size;
			std::vector<size_t> _second_level_index_free_bit_array; //�t���[�u���b�N���X�g���J�e�S���r�b�g
			size_t _first_level_index_free_bit_array; //�t���[�u���b�N���X�g���J�e�S���r�b�g

			void _init_free_block_list(size_t cate1_size, size_t cate2_size); //�t���[�u���b�N������
			void _push_free_block(_MemoryBlock* block); //�t���[�u���b�N�ǉ�
			_MemoryBlock* _pop_free_block(size_t cate1, size_t cate2); //�t���[�u���b�N����w�肳�ꂽ�J�e�S�����i�[�\�ȃu���b�N��Ԃ�
			void _remove_free_block(_MemoryBlock* block);
			_MemoryBlock* _marge_free_block(_MemoryBlock*); //�\�Ȃ�t���[�u���b�N����������(���̊֐����ł̓t���[�u���b�N���X�g�ւ̓o�^�͍s��Ȃ��B�}�[�W�����u���b�N�̓o�^�����͍s��)
			_MemoryBlock* _devide_free_block(_MemoryBlock*, size_t); //�\�Ȃ�t���[�u���b�N���w��T�C�Y�ɕ�������B���������E�u���b�N�ւ̃|�C���^��Ԃ�(���̊֐����ł̓t���[�u���b�N���X�g�ւ̓o�^�͍s��Ȃ�)

			size_t _second_level_index_separation_exponent_of_2; //���J�e�S���̕�������2�ׂ���
			size_t _second_level_index_separation; //���J�e�S���̕�����

			//�ŏ�ʃr�b�g�C���f�b�N�X�����߂�
			static size_t calcMSB(size_t size); //�ŏ�ʃr�b�g�����߂�
//				static size_t calcLSB(size_t size);
			size_t _calc_second_level_index(size_t size, size_t first_category); //���J�e�S�����v�Z����
			size_t _get_free_block_list_index(size_t cate1, size_t cate2); //�t���[�u���b�N���X�g�̃C���f�b�N�X���v�Z����B

			TLSFMemoryAllocator& _tlsf;
		};

		inline bool planeta_engine::core::TLSFMemoryAllocator::_Impl::reserve_memory(size_t size)
		{
			if (_tlsf._memory_pool) { free(_tlsf._memory_pool); } //�m�ۂ��Ă���̈�����
			_tlsf._allocated_memory_count = 0;
			_tlsf._allcated_memory_size = 0;
			_tlsf._using_memory_size = 0;
			//�Œ�Ǘ��̈�(���[�̃_�~�[�u���b�N�A��ڂ̃f�[�^�u���b�N�̍��v�̈�ƍŒ�f�[�^�̈�(���J�e�S��������))�ɑ���Ȃ�������G���[
			if (size <= _MemoryBlock::manage_area_size * 3 + _second_level_index_separation) { return false; }
			_tlsf._memory_pool = reinterpret_cast<char*>(std::malloc(size)); //�������m��
			_tlsf._memory_pool_size = size;
			//�O��Ƀ_�~�[�u���b�N���쐬
			_MemoryBlock* head_dumy = new(_tlsf._memory_pool)_MemoryBlock();
			_MemoryBlock* foot_dumy = new(reinterpret_cast<char*>(_tlsf._memory_pool) + _tlsf._memory_pool_size - _MemoryBlock::manage_area_size)_MemoryBlock();
			head_dumy->SetUsingFlag(true);
			foot_dumy->SetUsingFlag(true);
			_tlsf._using_memory_size += head_dumy->GetTotalSize();
			_tlsf._using_memory_size += foot_dumy->GetTotalSize();
			//�̈�Ɏ��܂�悤�Ƀ������u���b�N�쐬
			size_t create_block_size = (_tlsf._memory_pool_size - head_dumy->GetTotalSize() - foot_dumy->GetTotalSize());
			_MemoryBlock* block = new(reinterpret_cast<char*>(head_dumy) + head_dumy->GetTotalSize())_MemoryBlock(create_block_size, true);
			//�쐻�����f�[�^�̊i�[�J�e�S�������߂�
			size_t cate1 = calcMSB(block->GetDataSize() + 1);
			size_t cate2 = _calc_second_level_index(block->GetDataSize() + 1, cate1);
			if (cate2 == 0) { --cate1; cate2 = _second_level_index_separation - 1; }
			else { --cate2; }
			//�w�肳�ꂽ�T�C�Y���̃t���[�u���b�N���X�g�̈���m��
			_init_free_block_list(cate1 + 1, cate2 + 1);
			//�m�ۂ����������u���b�N���t���[�u���b�N���X�g�ɓo�^
			_push_free_block(block);
			return true;
		}

		inline TLSFMemoryAllocator::MemoryDetailScanResult TLSFMemoryAllocator::_Impl::detail_scan() const
		{
			MemoryDetailScanResult res;
			if (_tlsf._memory_pool == nullptr) { return res; }
			char* ptr = _tlsf._memory_pool;
			//�������u���b�N�T��
			while (ptr < _tlsf._memory_pool + _tlsf._memory_pool_size) {
				_MemoryBlock* block = reinterpret_cast<_MemoryBlock*>(ptr);
				if (block->ErrorCheck() || block->FooterErrorCheck()) { res.corruption = true; break; } //�G���[�`�F�b�N
				size_t data_size = block->GetDataSize();
				size_t total_size = block->GetTotalSize();
				bool use_flag = block->IsUsing();
				if (use_flag) { //�A�N�e�B�u�u���b�N
					if (data_size > 0) { ++res.active_block_count; }
					else { ++res.dumy_block_count; }
					res.allocated_memory_size += data_size;
					res.using_memory_size += total_size;
				}
				else { //�t���[�u���b�N
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
			//�t���[�u���b�N���X�g�̃X�L����
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
			//�G���[���o
			res.inconsistency_at_allocated_memory_count = (_tlsf._allocated_memory_count != res.active_block_count);
			res.inconsistency_at_allocated_memory_size = (_tlsf._allcated_memory_size != res.allocated_memory_size);
			res.inconsistency_at_reserved_memory_size = (res.reserved_memory_size != _tlsf._memory_pool_size);
			res.inconsistency_at_using_memory_size = (res.using_memory_size != _tlsf._using_memory_size);
			res.inconsistency_at_dumy_count = (res.dumy_block_count != 2);
			res.inconsistency_at_free_block_list = res.free_block_count != free_list_count;
			res.inconsistency_at_second_free_block_list_bit_array = (_first_level_index_free_bit_array != correct_first_free_block_bit_array);
			res.error = res.corruption || res.inconsistency_at_allocated_memory_count || res.inconsistency_at_allocated_memory_size || res.inconsistency_at_using_memory_size || res.inconsistency_at_dumy_count || res.inconsistency_at_free_block_list || res.inconsistency_at_first_free_block_list_bit_array || res.inconsistency_at_second_free_block_list_bit_array;
			//�f�Љ����v�Z
			res.fragment_ratio = 1.0 - ((double)res.available_max_memory_allocation_size / res.available_memory_size);
			return res;
		}

		inline void* TLSFMemoryAllocator::_Impl::alloc(size_t size)
		{
			if (size == 0) { throw BadAlloc("Size must not equal zero."); }
			//�v���������̃J�e�S�����m�F
			size_t cate_1 = calcMSB(size);
			size_t cate_2 = _calc_second_level_index(size, cate_1);
			_MemoryBlock* _allocated_block = _pop_free_block(cate_1, cate_2);
			if (_allocated_block == nullptr) {
				char err[256] = "\0";
				sprintf_s(err, 256, "failed to allocate memory. avalable free block is not exist. (size : %ul)", size);
				throw BadAlloc(err);
			}
			//�\�Ȃ番������
			_MemoryBlock* remain_block = _devide_free_block(_allocated_block, size);
			_allocated_block->SetUsingFlag(true);
			if (remain_block) { _push_free_block(remain_block); }

			_tlsf._allcated_memory_size += _allocated_block->GetDataSize();
			_tlsf._using_memory_size += _allocated_block->GetTotalSize();
			++_tlsf._allocated_memory_count;

			return _allocated_block->GetDataBlockPointer(); //�f�[�^�̈�̃|�C���^��Ԃ�
		}

		inline void TLSFMemoryAllocator::_Impl::dealloc(void* ptr)
		{
			//�w��|�C���^���m�ۗ̈�����m�F(���[�̃_�~�[�̈���͈͊O�Ƃ݂Ȃ�)
			if (ptr < (_tlsf._memory_pool + _MemoryBlock::manage_area_size) || ptr >= (_tlsf._memory_pool + _tlsf._memory_pool_size - _MemoryBlock::manage_area_size)) { throw BadAlloc("Designated pointer is out of range,"); }
			_MemoryBlock* _block = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(ptr) - sizeof(_MemoryBlock));
			if (_block->ErrorCheck()) { throw BadAlloc("Designated pointer is invalied."); } //�w��|�C���^�[���L�����m�F
			if (!_block->IsUsing()) { throw BadAlloc("Designated pointer is already deallocated."); } //�w��|�C���^�[���g�p�����m�F
			_tlsf._allcated_memory_size -= _block->GetDataSize();
			_tlsf._using_memory_size -= _block->GetTotalSize();
			--_tlsf._allocated_memory_count;

			_block->SetUsingFlag(false);
			//�K�v�Ȃ�}�[�W����
			_block = _marge_free_block(_block);
			//�t���[���X�g�ɒǉ�
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
			_free_block_list = reinterpret_cast<_MemoryBlock*>(malloc(sizeof(_MemoryBlock)*fbl_size)); //�̈�m�ۂ���
			for (unsigned int i = 0; i < fbl_size; ++i) {
				new(&_free_block_list[i]) _MemoryBlock(false); //�z�unew�ŏ�����
			}
			_free_block_list_size = fbl_size;
			_first_level_index_free_bit_array = 0;
			_second_level_index_free_bit_array.clear();
			_second_level_index_free_bit_array.resize(cate1_size, 0);
		}

		inline void TLSFMemoryAllocator::_Impl::_push_free_block(_MemoryBlock* block) {
			//�����̏����J�e�S��������ɂ����
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
			if (cate_2u < 0) { //�Ȃ������珊���\�ȑ��J�e�S����T��
				cate_1u = getFreeListSLI(cate_1, _first_level_index_free_bit_array);
				if (cate_1u >= 0) {
					cate_2u = getFreeListSLI(0, _second_level_index_free_bit_array[cate_1u]);
					if (cate_2u < 0) { throw std::logic_error("�t���[�u���b�N���X�g�r�b�g�̖���"); } //���蓖�Ď��s(���肦�Ȃ�)
				}
				else { return nullptr; } //���蓖�Ď��s
			}
			cate_1 = (unsigned)cate_1u;
			cate_2 = (unsigned)cate_2u;
			size_t free_list_idx = _get_free_block_list_index(cate_1, cate_2);
			if (free_list_idx >= _free_block_list_size) { throw std::logic_error("attempt to access out of free bit list! "); }
			_MemoryBlock* _allocated_block = _free_block_list[free_list_idx].next();
			_remove_free_block(_allocated_block); //�t���[�u���b�N���X�g���珜��
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
			// �ŏ�ʃr�b�g�����̃r�b�g�񂾂���L���ɂ���}�X�N
			const unsigned mask = (1 << first_category) - 1;   // 1000 0000 -> 0111 1111
			// �E�ւ̃V�t�g�����Z�o
			const unsigned rs = first_category - _second_level_index_separation_exponent_of_2;    // 7 - 3 = 4 �i8�����Ȃ�N=3�ł��j
			// ����size�Ƀ}�X�N�������āA�E�փV�t�g����΃C���f�b�N�X��
			return (size & mask) >> rs;
		}

		inline size_t TLSFMemoryAllocator::_Impl::_get_free_block_list_index(size_t cate1, size_t cate2)
		{
			return cate1 * (1u << _second_level_index_separation_exponent_of_2) + cate2 - 1;
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock* TLSFMemoryAllocator::_Impl::_marge_free_block(_MemoryBlock* block) {
			//�E
			_MemoryBlock* right = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(block) + block->GetTotalSize());
			if (!right->IsUsing()) {
				_remove_free_block(right);
				new(block)_MemoryBlock(block->GetTotalSize() + right->GetDataSize());
			}
			//��
			size_t tsize = *(reinterpret_cast<size_t*>(reinterpret_cast<char*>(block) - _MemoryBlock::footer_size));
			_MemoryBlock* left = reinterpret_cast<_MemoryBlock*>(reinterpret_cast<char*>(block) - tsize);
			if (!left->IsUsing()) {
				_remove_free_block(left);
				block = new(left)_MemoryBlock(left->GetTotalSize() + block->GetDataSize());
			}
			return block;
		}

		inline TLSFMemoryAllocator::_Impl::_MemoryBlock* TLSFMemoryAllocator::_Impl::_devide_free_block(_MemoryBlock* block, size_t size) {
			//������̎c��̈�T�C�Y���Œ�l��菬���������番�����Ȃ��m�ۂł��Ȃ��Ȃ�
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
			//�t�b�^�[�ɍ��v�T�C�Y��������
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
