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
			/*�Í���*/
			bool Encrypt(file_system::File& src_and_dst)const;
			bool Encrypt(const file_system::File& src, file_system::File& dst)const;
			/*������*/
			bool Decrypt(file_system::File& src_and_dst)const;
			bool Decrypt(const file_system::File& src, file_system::File& dst)const;
			/**
			* @brief �ꕔ������
			* @param src:��������,pos:�������擪�ʒu,size:�������T�C�Y,dst:�������f�[�^�̊i�[��
			* @return ����������
			*/
			bool DecryptPartially(const file_system::File& src,size_t pos,size_t size, file_system::File& dst)const;
			/**
			* @brief �ꕔ������(�^�w��^�C�v�B�w�肵���^�̕���������������)
			* @param src:��������,pos:�������擪�ʒu,dst:�������f�[�^�̊i�[��
			* @return ����������
			*/
			template<typename T>
			bool DecryptPartially(const file_system::File& src, size_t pos, T& dst)const {
				return DecryptPartiallyCore(src, pos, sizeof(T), reinterpret_cast<char*>(&dst), sizeof(T));
			}
		private:
			/*�������������s���B�Ή����Ȃ��ꍇ�̓I�[�o�[���C�h���Ȃ��ėǂ��Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool DecryptPartiallyCore(const file_system::File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size)const { return false; };
			/*�Í������s���Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool EncryptCore(const file_system::File& src, file_system::File& dst)const = 0;
			/*���������s���Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool DecryptCore(const file_system::File& src, file_system::File& dst)const = 0;
		};
	}
}
