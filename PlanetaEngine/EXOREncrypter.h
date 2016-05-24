#pragma once

#include <cstdint>
#include "EncrypterBase.h"

namespace planeta_engine {
	namespace encrypters {
		class EXOREncrypter final: public EncrypterBase {
		public:
			explicit EXOREncrypter(uint_fast64_t key) :key_(key) { CalculateKey(); }
		private:
			static const size_t block_byte_size = 32 / 4;
			uint_fast64_t key_;
			uint_fast32_t main_key_;
			uint_fast32_t sub_key_;
			uint_fast32_t block_key(size_t block)const;
			void CalculateKey();
			bool DecryptPartiallyCore(const File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size) const override;
			bool EncryptCore(const File& src, File& dst) const override;
			bool DecryptCore(const File& src, File& dst) const override;

		};
	}
}