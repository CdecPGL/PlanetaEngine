#include "encrypter_base.hpp"

#include "planeta/core/file.hpp"

namespace plnt::archiver {
	bool encrypter_base::encrypt(const file &src, file &dst) const {
		if (&src == &dst) { return encrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
		return encrypt_core(src, dst);
	}

	bool encrypter_base::encrypt(file &src_and_dst) const {
		if (file dst; encrypt_core(src_and_dst, dst)) {
			src_and_dst = std::move(dst);
			return true;
		} //一時デスティネーションを入力に移動する
		return false;
	}

	bool encrypter_base::decrypt(file &src_and_dst) const {
		if (file dst; decrypt_core(src_and_dst, dst)) {
			src_and_dst = std::move(dst);
			return true;
		} //一時デスティネーションを入力に移動する
		return false;
	}

	bool encrypter_base::decrypt(const file &src, file &dst) const {
		if (&src == &dst) { return decrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
		return decrypt_core(src, dst);
	}

	bool encrypter_base::decrypt_partially(const file &src, const size_t pos, const size_t size, file &dst) const {
		if (!dst.reserve(size, false)) { return false; }
		if (&src == &dst) { return false; } //ソースとデスティネーションが同じではいけない。
		return decrypt_partially_core(src, pos, size, reinterpret_cast<char *>(dst.top_pointer()), dst.size());
	}
}
