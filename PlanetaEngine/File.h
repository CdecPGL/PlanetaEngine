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
			/*�g���q�Z�b�g�ŃR���X�g���N�^*/
			File(const std::string&);
			~File();
			std::string GetExtension()const;
			void UnloadData();
			unsigned int GetSize()const;
			const void* const GetTopPointer()const;
			FileStatus GetStatus()const;
			/*�f�[�^�Z�b�g(�擪�A�h���X�A�T�C�Y�A�Í����f�[�^��)*/
			void SetData(void*, unsigned int, bool = false);
			/*�G���[����*/
			void ErrorOccured();

		private:
			const std::string extension;
			FileStatus status = FileStatus::Unloaded;
			void* data_top = nullptr;
			unsigned int size = 0;
		};
	}
}
