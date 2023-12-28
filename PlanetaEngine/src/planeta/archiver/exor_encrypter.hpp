#pragma once

#include <cstdint>

#include "encrypter_base.hpp"

namespace plnt::archiver {
	class exor_encrypter final : public encrypter_base {
	public:
		explicit exor_encrypter(const uint_fast64_t key)
			: key_(key), main_key_{}, sub_key_{} { calculate_key(); }

	private:
		static constexpr size_t block_byte_size = 32 / 4;
		uint_fast64_t key_;
		uint_fast32_t main_key_;
		uint_fast32_t sub_key_;
		static uint_fast32_t block_key(size_t block);
		void calculate_key();
		bool decrypt_partially_core(const file &src, size_t pos, size_t size, char *dst_ptr,
		                            size_t dst_size) const override;
		bool encrypt_core(const file &src, file &dst) const override;
		bool decrypt_core(const file &src, file &dst) const override;
	};
}
