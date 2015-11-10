#pragma once

#include<memory>
#include<string>
#include "Object.h"
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class EncrypterBase;
		class FileLoaderBase : public core::Object{
		public:
			explicit FileLoaderBase(const std::string& p) :path_(p),is_valid_(false){}
			explicit FileLoaderBase(const std::string& p, const std::shared_ptr<const EncrypterBase>& encrypter) :path_(p), encrypter_(encrypter), is_valid_(false) {}
			virtual ~FileLoaderBase() = default;
			bool Initialize() {
				if (is_valid_) { return false; }
				if (_Initialize()) { is_valid_ = true; return true; }
				else { return false; }
			}
			void Finalize() {
				_Finalize();
				is_valid_ = false;
			}
			/*�t�@�C���̓ǂݍ��݁B�L���b�V��������ꍇ�̓L���b�V���𗘗p*/
			virtual std::shared_ptr<File> LoadFile(const std::string& name) = 0;
			/*�L���b�V���f�[�^�̏���*/
			virtual bool DeleteCache() { return true; }
			/*�S�t�@�C���f�[�^���L���b�V���Ƃ��ēǂݍ���(�ǂݍ��ݍς݂̂��̂̓X�L�b�v)*/
			virtual bool LoadAllFileToCache() { return true; }
			/*�t�@�C�����X�g���X�V(�L���b�V���͑S��������܂�)*/
			virtual bool UpdateFileList() = 0;
			/*�L���b�V�����ꂽ�f�[�^�̃T�C�Y*/
			virtual size_t GetCacheSize()const { return 0; }
			bool is_valid()const { return is_valid_; }
		protected:
			virtual bool _Initialize() = 0;
			virtual void _Finalize() = 0;
			const std::string& path()const { return path_; }
			void path(const std::string& p) { path_ = p; }
			bool is_encrypter_valid()const { return encrypter_ != nullptr; }
			std::shared_ptr<const EncrypterBase> encrypter()const { return encrypter_; }
		private:
			std::string path_;
			bool is_valid_; //�L����
			std::shared_ptr<const EncrypterBase> encrypter_;
		};
	}
}
