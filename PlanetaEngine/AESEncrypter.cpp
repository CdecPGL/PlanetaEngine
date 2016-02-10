#include "AESEncrypter.h"

#include <array>

#include "cryptlib.h"
#include "aes.h"
#include "dh.h"
#include "modes.h"
#include "osrng.h"

#include "File.h"

namespace planeta_engine {
	namespace encrypters {
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////

		class AESEncrypter::Impl_ {
		private:
			std::unique_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption> aes_enc_obj_;
			std::unique_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption> aes_dec_obj_;
			bool enc_obj_update = false;
			bool dec_obj_update = false;
			std::array<byte, 16> key_; //128bit
			std::array<byte, CryptoPP::AES::BLOCKSIZE> iv_; //128bit

			void UpdateEncObj() {
				if (aes_enc_obj_ == nullptr) {
					aes_enc_obj_ = std::make_unique<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>();
				}
				if (enc_obj_update) {
					aes_enc_obj_->SetKeyWithIV(key_.data(), key_.size(), iv_.data());
					enc_obj_update = false;
				}
			}

			void UpdateDecObj() {
				if (aes_enc_obj_ == nullptr) {
					aes_dec_obj_ = std::make_unique<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>();
				}
				if (dec_obj_update) {
					aes_dec_obj_->SetKeyWithIV(key_.data(), key_.size(), iv_.data());
					dec_obj_update = false;
				}
			}

			template<typename T, int S>
			static void SetByteArrayFromUint128(const uint128_t& src, std::array<T, S>& dst) {
				for (int i = 0; i < S && (i + 1)*sizeof(T) <= 16; ++i) {
					dst[i] = static_cast<T>(src << i*sizeof(T));
				}
			}
		public:
			bool SetKey(const uint128_t& key, const uint128_t& initializatio_vector) {
				SetByteArrayFromUint128(key, key_);
				SetByteArrayFromUint128(initializatio_vector, iv_);
				enc_obj_update = true;
				dec_obj_update = true;
				return true;
			}

			bool Encrypt(file_system::File& dst,const file_system::File& src) {
				UpdateEncObj();
				std::string dat;
				CryptoPP::StreamTransformationFilter enc_filter(*aes_enc_obj_, new CryptoPP::StringSink(dat));
				enc_filter.Put(src.GetTopPointer(), src.GetSize());
				enc_filter.MessageEnd();
				dst.SetData(dat);
				return true;
			}

			bool Decrypt(file_system::File& dst, const file_system::File& src) {
				UpdateDecObj();
				std::string dat;
				CryptoPP::StreamTransformationFilter dec_filter(*aes_dec_obj_, new CryptoPP::StringSink(dat));
				dec_filter.Put(src.GetTopPointer(), src.GetSize());
				dec_filter.MessageEnd();
				dst.SetData(dat);
				return true;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		//AESEnctypter
		//////////////////////////////////////////////////////////////////////////

		AESEncrypter::AESEncrypter() :impl_(std::make_unique<Impl_>()) {};
		AESEncrypter::~AESEncrypter() = default;

		bool AESEncrypter::EncryptCore(const file_system::File& src, file_system::File& dst) const {
			return impl_->Encrypt(dst, src);
		}

		bool AESEncrypter::DecryptCore(const file_system::File& src, file_system::File& dst) const {
			return impl_->Decrypt(dst, src);
		}

		bool AESEncrypter::SetKey(const uint128_t& key, const uint128_t& initializatio_vector) {
			return impl_->SetKey(key, initializatio_vector);
		}

	}
}