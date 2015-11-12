#pragma once

#include<memory>
#include<string>
#include <unordered_set>
#include "Object.h"
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class EncrypterBase;
		class FileLoaderBase: public core::Object{
		public:
			explicit FileLoaderBase(const std::string& p) :path_(p),is_valid_(false){}
			explicit FileLoaderBase(const std::string& p, const std::shared_ptr<const EncrypterBase>& encrypter) :path_(p), encrypter_(encrypter), is_valid_(false) {}
			virtual ~FileLoaderBase() = default;
			bool Initialize();
			void Finalize() {
				if (!is_valid_) { return; }
				FinalizeCore();
				is_valid_ = false;
			}
			/*ファイルの読み込み*/
			std::shared_ptr<File> LoadFile(const std::string& name);
			/*すべてのファイルを読み込み*/
			std::vector<std::pair<std::string, std::shared_ptr<File>>> LoadAllFiles();
			/*ファイルリストを更新*/
			bool UpdateFileList() { file_list_.clear(); return UpdateFileListCore(file_list_); }
			/*ファイルの存在を確認*/
			bool CheckFileExist(const std::string& file_name)const;
			bool is_valid()const { return is_valid_; }
			const std::unordered_set<std::string>& file_list()const { return file_list_; }
		protected:
			const std::string& path()const { return path_; }
			void path(const std::string& p) { path_ = p; }
			bool is_encrypter_valid()const { return encrypter_ != nullptr; }
			std::shared_ptr<const EncrypterBase> encrypter()const { return encrypter_; }
		private:
			std::string path_;
			bool is_valid_; //有効か
			std::shared_ptr<const EncrypterBase> encrypter_;
			std::unordered_set<std::string> file_list_;
			virtual bool InitializeCore() = 0;
			virtual void FinalizeCore() = 0;
			virtual bool UpdateFileListCore(std::unordered_set<std::string>& file_list) = 0;
			virtual bool LoadFileCore(const std::string& name,File& file) = 0;
			virtual bool LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files);
		};
	}
}
