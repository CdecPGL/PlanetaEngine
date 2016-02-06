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
			/*ファイルの読み込み*/
			std::shared_ptr<File> LoadFile(const std::string& name);
			/*すべてのファイルを読み込み*/
			std::vector<std::pair<std::string, std::shared_ptr<File>>> LoadAllFiles();
			/*ファイルリストを更新*/
			bool UpdateFileList() { file_list_.clear(); return UpdateFileListCore(file_list_); }
			/*ファイルの保存(保存名、ファイル)*/
			bool SaveFile(const std::string& name, const File& file);
			/*複数ファイルの保存*/
			bool SaveFiles(const std::vector<std::pair<std::string, const File&>>& files);
			/*ファイルの存在を確認*/
			bool CheckFileExist(const std::string& file_name)const;
			/*ファイルの削除*/
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
			bool is_valid_; //有効か
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
//			virtual bool DeleteFileCore(const std::string& file_name) { return false; }; //未実装
		};
	}
}
