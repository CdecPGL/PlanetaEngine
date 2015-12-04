#include "EXOREncrypter.h"
#include "File.h"

/*データ構造について
先頭32bitにはファイルサイズが記録されている。
それも含めて全体が32bit単位で暗号化される。
*/

namespace {
	constexpr size_t HEADER_BYTE_SIZE(4);
}

namespace planeta_engine {
	namespace encrypters {
		bool EXOREncrypter::DecryptPartiallyCore(const file_system::File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size) const
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool EXOREncrypter::EncryptCore(const file_system::File& src, file_system::File& dst) const
		{
			int b_key = main_key_;
			uint_fast32_t buf;
			size_t block_num = (src.GetSize() + block_byte_size - 1) / block_byte_size; //必要ブロック数
			//ディスティネーションのサイズ変更
			dst.ChangeSize(block_num*block_byte_size + HEADER_BYTE_SIZE);
			//サイズ書き込み
			buf = src.GetSize() ^ b_key;
			dst.WriteData(0, buf);
			//暗号化しつつ書き込み
			for (size_t block = 0; block < block_num; ++block, b_key += sub_key_) {
				src.ReadData(block*block_byte_size, buf);
				buf ^= b_key;
				dst.WriteData(block*block_byte_size + HEADER_BYTE_SIZE, buf, false);
			}
			return true;
		}

		bool EXOREncrypter::DecryptCore(const file_system::File& src, file_system::File& dst) const
		{
			int b_key = main_key_;
			uint_fast32_t buf;
			//サイズ取得
			src.ReadData(0, buf);
			uint_fast32_t size = buf ^ b_key; 
			//ブロック数計算
			if ((src.GetSize() - HEADER_BYTE_SIZE) % block_byte_size != 0) { //ブロックが合わない
				return false;
			}
			size_t block_num = (src.GetSize() - HEADER_BYTE_SIZE) / block_byte_size;
			//ディスティネーションのサイズ変更
			dst.ChangeSize(size, false);
			//復号化しつつ書き込み
			for (size_t block = 0; block < block_num; ++block,b_key += sub_key_) {
				src.ReadData(block*block_byte_size+HEADER_BYTE_SIZE, buf);
				buf ^= b_key;
				if (block == block_num - 1) { //最終ブロックだったら
					size_t rest = (block_num - 1)*block_byte_size - size; //残りのデータサイズ
					dst.WriteData(block*block_byte_size, reinterpret_cast<unsigned char*>(&buf), rest, false);
				}
				else {
					dst.WriteData(block*block_byte_size, buf, false);
				}
			}
			return true;
		}

		uint_fast32_t EXOREncrypter::block_key(size_t block) const
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		void EXOREncrypter::CalculateKey()
		{
			main_key_ = static_cast<uint_fast32_t>(key_); //keyの下32bit
			sub_key_ = static_cast<uint_fast32_t>(key_ >> 32); //keyの上32bit
		}

	}
}
