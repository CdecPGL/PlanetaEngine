#pragma once

#include<memory>
#include<string>
#include <unordered_set>
#include "Object.h"
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class EncrypterBase;
		class FileManipulatorBase: public core::Object{
		public:
			explicit FileManipulatorBase(const std::string& p, bool auto_create) :path_(p), is_valid_(false), auto_create_(auto_create) {}
			explicit FileManipulatorBase(const std::string& p, std::shared_ptr<const EncrypterBase>&& encrypter, bool auto_create) :path_(p), encrypter_(std::move(encrypter)), is_valid_(false), auto_create_(auto_create) {}
			virtual ~FileManipulatorBase() = default;
			bool Initialize();
			void Finalize() {
				if (!is_valid_) { return; }
				FinalizeCore();
				is_valid_ = false;
			}
			/*�t�@�C���̓ǂݍ���*/
			std::shared_ptr<File> LoadFile(const std::string& name);
			/*���ׂẴt�@�C����ǂݍ���*/
			std::vector<std::pair<std::string, std::shared_ptr<File>>> LoadAllFiles();
			/*�t�@�C�����X�g���X�V*/
			bool UpdateFileList() { file_list_.clear(); return UpdateFileListCore(file_list_); }
			/*�t�@�C���̕ۑ�(�ۑ����A�t�@�C��)*/
			bool SaveFile(const std::string& name, const File& file);
			/*�����t�@�C���̕ۑ�*/
			bool SaveFiles(const std::vector<std::pair<std::string, const File&>>& files);
			/*�t�@�C���̑��݂��m�F*/
			bool CheckFileExist(const std::string& file_name)const;
			/*�t�@�C���̍폜*/
//			bool DeleteFile(const std::string& file_name);
			bool is_valid()const { return is_valid_; }
			const std::unordered_set<std::string>& file_list()const { return file_list_; }
		protected:
			const std::string& path()const { return path_; }
			void path(const std::string& p) { path_ = p; }
			bool is_encrypter_valid()const { return encrypter_ != nullptr; }
			std::shared_ptr<const EncrypterBase> encrypter()const { return encrypter_; }
			bool auto_create()const { return auto_create_; }
		private:
			std::string path_;
			bool is_valid_; //�L����
			bool auto_create_;
			std::shared_ptr<const EncrypterBase> encrypter_;
			std::unordered_set<std::string> file_list_;
			virtual bool InitializeCore() = 0;
			virtual void FinalizeCore() = 0;
			virtual bool UpdateFileListCore(std::unordered_set<std::string>& file_list) = 0;
			virtual bool LoadFileCore(const std::string& name,File& file) = 0;
			virtual bool LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files);
			virtual bool SaveFileCore(const std::string& name, const File& file) = 0;
			virtual bool SaveFilesCore(const std::vector<std::pair<std::string, const File&>>& files);
//			virtual bool DeleteFileCore(const std::string& file_name) { return false; }; //������
		};
	}
}
