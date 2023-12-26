#pragma once

#include <memory>

#include "boost/multiprecision/cpp_int.hpp"

#include "encrypter_base.hpp"

namespace plnt::archiver {
	//128bit鍵長AES暗号器
	class aes_encrypter final : public encrypter_base {
	public:
		aes_encrypter();
		using uint128_t = boost::multiprecision::uint128_t;
		[[nodiscard]] bool set_key(const uint128_t &key, const uint128_t &initialization_vector) const;

	private:
		bool encrypt_core(const File &src, File &dst) const override;
		bool decrypt_core(const File &src, File &dst) const override;

		class impl;
		std::unique_ptr<impl> impl_;
	};
}
