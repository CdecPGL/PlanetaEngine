#pragma once

#include "Object.h"

namespace planeta_engine {
	namespace file_system {
		class File;
	}
	namespace encrypters {
		class EncrypterBase {
		public:
			EncrypterBase() = default;
			~EncrypterBase() = default;
			/*暗号化*/
			bool Encrypt(file_system::File& src_and_dst)const;
			bool Encrypt(const file_system::File& src, file_system::File& dst)const;
			/*復号化*/
			bool Decrypt(file_system::File& src_and_dst)const;
			bool Decrypt(const file_system::File& src, file_system::File& dst)const;
			/**
			* @brief 一部復号化
			* @param src:復号化元,pos:復号化先頭位置,size:復号化サイズ,dst:復号化データの格納先
			* @return 成功したか
			*/
			bool DecryptPartially(const file_system::File& src,size_t pos,size_t size, file_system::File& dst)const;
			/**
			* @brief 一部復号化(型指定タイプ。指定した型の分だけ復号化する)
			* @param src:復号化元,pos:復号化先頭位置,dst:復号化データの格納先
			* @return 成功したか
			*/
			template<typename T>
			bool DecryptPartially(const file_system::File& src, size_t pos, T& dst)const {
				return DecryptPartiallyCore(src, pos, sizeof(T), reinterpret_cast<char*>(&dst), sizeof(T));
			}
		private:
			/*部分復号化を行う。対応しない場合はオーバーライドしなくて良い。srcとdstが異なることは保証される。*/
			virtual bool DecryptPartiallyCore(const file_system::File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size)const { return false; };
			/*暗号化を行う。srcとdstが異なることは保証される。*/
			virtual bool EncryptCore(const file_system::File& src, file_system::File& dst)const = 0;
			/*復号化を行う。srcとdstが異なることは保証される。*/
			virtual bool DecryptCore(const file_system::File& src, file_system::File& dst)const = 0;
		};
	}
}
