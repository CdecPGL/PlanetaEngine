#pragma once

#include<memory>
#include<string>
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class LoaderBase{
		public:
			LoaderBase(const std::string& p) :_path(p),_is_valid(false){}
			virtual ~LoaderBase() = default;
			bool Initialize() {
				if (_is_valid) { return false; }
				if (_Initialize()) { _is_valid = true; return true; }
				else { return false; }
			}
			void Finalize() {
				_Finalize();
				_is_valid = false;
			}
			/*�t�@�C���̓ǂݍ��݁B�L���b�V��������ꍇ�̓L���b�V���𗘗p*/
			virtual std::shared_ptr<File> LoadFile(const std::string&) = 0;
			/*�L���b�V���f�[�^�̏���*/
			virtual int DeleteCache() = 0;
			/*�S�t�@�C���f�[�^���L���b�V���Ƃ��ēǂݍ���(�ǂݍ��ݍς݂̂��̂̓X�L�b�v)*/
			virtual int LoadAllFileToCache() = 0;
			/*�t�@�C�����X�g���X�V(�L���b�V���͑S��������܂�)*/
			virtual int UpdateFileList() = 0;
			/*�L���b�V�����ꂽ�f�[�^�̃T�C�Y*/
			virtual unsigned int GetCacheSize()const = 0;
			bool IsValid()const { return _is_valid; }
		protected:
			virtual bool _Initialize() = 0;
			virtual void _Finalize() = 0;
			std::string _path;
		private:
			bool _is_valid; //�L����
		};
	}
}
