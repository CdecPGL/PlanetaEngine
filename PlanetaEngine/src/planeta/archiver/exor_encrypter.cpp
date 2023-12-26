﻿#include "planeta/core/File.hpp"

#include "exor_encrypter.hpp"

/*データ構造について
先頭32bitにはファイルサイズが記録されている。
それも含めて全体が32bit単位で暗号化される。
*/

namespace {
	constexpr size_t header_byte_size(4);
}

namespace plnt::archiver {
	bool exor_encrypter::decrypt_partially_core(const File &src, size_t pos, size_t size, char *dst_ptr,
	                                          size_t dst_size) const {
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool exor_encrypter::encrypt_core(const File &src, File &dst) const {
		auto b_key = main_key_;
		const size_t block_num = (src.size() + block_byte_size - 1) / block_byte_size; //必要ブロック数
		//ディスティネーションのサイズ変更
		dst.Reserve(block_num * block_byte_size + header_byte_size);
		//サイズ書き込み
		uint_fast32_t buf = src.size() ^ b_key;
		dst.WriteData(0, buf);
		//暗号化しつつ書き込み
		for (size_t block = 0; block < block_num; ++block, b_key += sub_key_) {
			src.ReadData(block * block_byte_size, buf);
			buf ^= b_key;
			dst.WriteData(block * block_byte_size + header_byte_size, buf, false);
		}
		return true;
	}

	bool exor_encrypter::decrypt_core(const File &src, File &dst) const {
		auto b_key = main_key_;
		uint_fast32_t buf;
		//サイズ取得
		src.ReadData(0, buf);
		const uint_fast32_t size = buf ^ b_key;
		//ブロック数計算
		if ((src.size() - header_byte_size) % block_byte_size != 0) {
			//ブロックが合わない
			return false;
		}
		const size_t block_num = (src.size() - header_byte_size) / block_byte_size;
		//ディスティネーションのサイズ変更
		dst.Reserve(size, false);
		//復号化しつつ書き込み
		for (size_t block = 0; block < block_num; ++block, b_key += sub_key_) {
			src.ReadData(block * block_byte_size + header_byte_size, buf);
			buf ^= b_key;
			if (block == block_num - 1) {
				//最終ブロックだったら
				const size_t rest = (block_num - 1) * block_byte_size - size; //残りのデータサイズ
				dst.WriteData(block * block_byte_size, reinterpret_cast<unsigned char *>(&buf), rest, false);
			} else { dst.WriteData(block * block_byte_size, buf, false); }
		}
		return true;
	}

	uint_fast32_t exor_encrypter::block_key(size_t block) {
		throw std::logic_error("The method or operation is not implemented.");
	}

	void exor_encrypter::calculate_key() {
		main_key_ = static_cast<uint_fast32_t>(key_); //keyの下32bit
		sub_key_ = static_cast<uint_fast32_t>(key_ >> 32); //keyの上32bit
	}
}
