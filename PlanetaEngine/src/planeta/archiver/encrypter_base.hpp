#pragma once

namespace plnt {
	class File;

	namespace archiver {
		class encrypter_base {
		public:
			encrypter_base() = default;
			virtual ~encrypter_base() = default;
			encrypter_base(const encrypter_base &) = default;
			encrypter_base(encrypter_base &&) = default;
			encrypter_base &operator =(const encrypter_base &) = default;
			encrypter_base &operator=(encrypter_base &&) = default;

			/*暗号化*/
			bool encrypt(File &src_and_dst) const;
			bool encrypt(const File &src, File &dst) const;
			/*復号化*/
			bool decrypt(File &src_and_dst) const;
			bool decrypt(const File &src, File &dst) const;
			/**
			* @brief 一部復号化
			* @param src 復号化元
			* @param pos 復号化先頭位置
			* @param size 復号化サイズ
			* @param dst 復号化データの格納先
			* @return 成功したか
			*/
			bool decrypt_partially(const File &src, size_t pos, size_t size, File &dst) const;
			/**
			* @brief 一部復号化(型指定タイプ。指定した型の分だけ復号化する)
			* @param src 復号化元
			* @param pos 復号化先頭位置
			* @param dst 復号化データの格納先
			* @return 成功したか
			*/
			template <typename T>
			bool decrypt_partially(const File &src, const size_t pos, T &dst) const {
				return decrypt_partially_core(src, pos, sizeof(T), reinterpret_cast<char *>(&dst), sizeof(T));
			}

		private:
			/*部分復号化を行う。対応しない場合はオーバーライドしなくて良い。srcとdstが異なることは保証される。*/
			virtual bool decrypt_partially_core(const File &src, size_t pos, size_t size, char *dst_ptr,
			                                    size_t dst_size) const { return false; }

			/*暗号化を行う。srcとdstが異なることは保証される。*/
			virtual bool encrypt_core(const File &src, File &dst) const = 0;
			/*復号化を行う。srcとdstが異なることは保証される。*/
			virtual bool decrypt_core(const File &src, File &dst) const = 0;
		};
	}
}
