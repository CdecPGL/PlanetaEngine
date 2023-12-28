#include <array>

#include "cryptopp/cryptlib.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"

#include "planeta/core/file.hpp"

#include "aes_encrypter.hpp"

namespace plnt::archiver {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class aes_encrypter::impl final {
		std::unique_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption> aes_enc_obj_;
		std::unique_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption> aes_dec_obj_;
		bool enc_obj_update_ = false;
		bool dec_obj_update_ = false;
		std::array<CryptoPP::byte, 16> key_ = {}; //128bit
		std::array<CryptoPP::byte, CryptoPP::AES::BLOCKSIZE> iv_ = {}; //128bit

		void update_enc_obj() {
			if (aes_enc_obj_ == nullptr) {
				aes_enc_obj_ = std::make_unique<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>();
			}
			if (enc_obj_update_) {
				aes_enc_obj_->SetKeyWithIV(key_.data(), key_.size(), iv_.data());
				enc_obj_update_ = false;
			}
		}

		void update_dec_obj() {
			if (aes_enc_obj_ == nullptr) {
				aes_dec_obj_ = std::make_unique<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>();
			}
			if (dec_obj_update_) {
				aes_dec_obj_->SetKeyWithIV(key_.data(), key_.size(), iv_.data());
				dec_obj_update_ = false;
			}
		}

		template <typename T, int S>
		static void set_byte_array_from_uint128(const uint128_t &src, std::array<T, S> &dst) {
			for (int i = 0; i < S && (i + 1) * sizeof(T) <= 16; ++i) { dst[i] = static_cast<T>(src << i * sizeof(T)); }
		}

	public:
		bool set_key(const uint128_t &key, const uint128_t &initialization_vector) {
			set_byte_array_from_uint128(key, key_);
			set_byte_array_from_uint128(initialization_vector, iv_);
			enc_obj_update_ = true;
			dec_obj_update_ = true;
			return true;
		}

		bool encrypt(file &dst, const file &src) {
			update_enc_obj();
			std::string dat;
			CryptoPP::StreamTransformationFilter enc_filter(*aes_enc_obj_, new CryptoPP::StringSink(dat));
			enc_filter.Put(src.top_pointer(), src.size());
			enc_filter.MessageEnd();
			dst.set_data(dat);
			return true;
		}

		bool decrypt(file &dst, const file &src) {
			update_dec_obj();
			std::string dat;
			CryptoPP::StreamTransformationFilter dec_filter(*aes_dec_obj_, new CryptoPP::StringSink(dat));
			dec_filter.Put(src.top_pointer(), src.size());
			dec_filter.MessageEnd();
			dst.set_data(dat);
			return true;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//AESEncrypter
	//////////////////////////////////////////////////////////////////////////

	aes_encrypter::aes_encrypter() : impl_(std::make_unique<impl>()) { };

	bool aes_encrypter::encrypt_core(const file &src, file &dst) const { return impl_->encrypt(dst, src); }

	bool aes_encrypter::decrypt_core(const file &src, file &dst) const { return impl_->decrypt(dst, src); }

	bool aes_encrypter::set_key(const uint128_t &key, const uint128_t &initialization_vector) const {
		return impl_->set_key(key, initialization_vector);
	}
}
