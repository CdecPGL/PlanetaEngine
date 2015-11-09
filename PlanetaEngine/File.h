#pragma once

#include "Object.h"
#include<string>
#include<vector>
#include <memory.h>

namespace planeta_engine{
	namespace file_system{
		class File : public core::Object
		{
		public:
			/*�t�@�C���̏��(���p�\�A�Í�������Ă���A���[�h����Ă��Ȃ��A��蔭��)*/
			enum class FileStatus { Available, Encrypted, Unloaded, Error };
			File();
			File(const File&);
			File(File&&);
			/*�g���q�Z�b�g�ŃR���X�g���N�^*/
			File(const std::string&);
			~File();
			File& operator=(const File&);
			File& operator=(File&&);
			std::string GetExtension()const;
			void UnloadData();
			unsigned int GetSize()const;
			const unsigned char* GetTopPointer()const;
			unsigned char* GetTopPointer();
			FileStatus GetStatus()const;
			/*�f�[�^�̃Z�b�g(�R�s�[����̂ł͂Ȃ��Ǘ����Ϗ�����)*/
			void SetData(unsigned char* top_ptr, size_t size);
			/*�f�[�^�̏�������*/
			bool WriteData(size_t pos, unsigned char* data_top, size_t data_size,bool auto_extend = false);
			template<typename T>
			bool WriteData(size_t pos,const T& d,bool auto_extend = true) {
				return WriteData(pos, reinterpret_cast<unsigned char*>(&T), sizeof(T), auto_extend);
			}
			/*�f�[�^�̓ǂݍ���*/
			bool ReadData(size_t pos, unsigned char* buffer_top, size_t buffer_size)const;
			template<typename T>
			bool ReadData(size_t pos,T& b) {
				return ReadData(pos, reinterpret_cast<unsigned char*>(&T), sizeof(T));
			}
			/*�T�C�Y�̕ύX*/
			bool ChangeSize(size_t size,bool copy = true);
			/*�G���[����*/
			void ErrorOccured();

		private:
			std::string extension_;
			FileStatus status_ = FileStatus::Unloaded;
			unsigned char* data_top_ = nullptr;
			unsigned int size_ = 0;
		};
	}
}
