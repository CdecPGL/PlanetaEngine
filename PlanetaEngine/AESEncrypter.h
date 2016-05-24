#pragma once

#include <memory>
#include <array>
#include "EncrypterBase.h"
#include "boost/multiprecision/cpp_int.hpp"

namespace planeta_engine{
	namespace encrypters {
		//128bitåÆí∑AESà√çÜäÌ
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