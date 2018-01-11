#pragma once

#include <memory>
#include <array>

#include "boost/multiprecision/cpp_int.hpp"

#include "EncrypterBase.hpp"

namespace plnt{
	namespace encrypters {
		//128bit鍵長AES暗号器
		class AESEncrypter final : public EncrypterBase {
		public:
			AESEncrypter();
			~AESEncrypter();
			using uint128_t = boost::multiprecision::uint128_t;
			bool SetKey(const uint128_t& key, const uint128_t& initialization_vector);
		private:
			bool EncryptCore(const File& src, File& dst) const override;
			bool DecryptCore(const File& src, File& dst) const override;

			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}