#pragma once

#include "Object.h"

namespace planeta_engine {
	namespace file_system {
		class File;
		class EncrypterBase {
		public:
			/*�Í���*/
			bool Encrypt(File& src_and_dst)const;
			bool Encrypt(const File& src, File& dst)const;
			/*������*/
			bool Decrypt(File& src_and_dst)const;
			bool Decrypt(const File& src, File& dst)const;
			/**
			* @brief �ꕔ������
			* @param src:��������,pos:�������擪�ʒu,size:�������T�C�Y,dst:�������f�[�^�̊i�[��
			* @return ����������
			*/
			bool DecryptPartially(const File& src,size_t pos,size_t size,File& dst)const;
			/**
			* @brief �ꕔ������(�^�w��^�C�v�B�w�肵���^�̕���������������)
			* @param src:��������,pos:�������擪�ʒu,dst:�������f�[�^�̊i�[��
			* @return ����������
			*/
			template<typename T>
			bool DecryptPartially(const File& src, size_t pos, T& dst)const {
				return DecryptPartiallyCore(src, pos, sizeof(T), reinterpret_cast<char*>(&dst), sizeof(T));
			}
		private:
			/*�������������s���B�Ή����Ȃ��ꍇ�̓I�[�o�[���C�h���Ȃ��ėǂ��Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool DecryptPartiallyCore(const File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size)const { return false; };
			/*�Í������s���Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool EncryptCore(const File& src, File& dst)const = 0;
			/*���������s���Bsrc��dst���قȂ邱�Ƃ͕ۏ؂����B*/
			virtual bool DecryptCore(const File& src, File& dst)const = 0;
		};
	}
}
