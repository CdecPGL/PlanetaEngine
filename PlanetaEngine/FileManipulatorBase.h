#pragma once

#include<memory>
#include<string>
#include <unordered_set>
#include "Object.h"
#include"File.h"
#include "boost/optional.hpp"

namespace planeta {
	namespace encrypters {
		class EncrypterBase;
	}
	class FileManipulatorBase : public Object {
	public:
		explicit FileManipulatorBase(const std::string& p, bool auto_create);
		explicit FileManipulatorBase(const std::string& p, std::unique_ptr<const encrypters::EncrypterBase>&& encrypter, bool auto_create);
		virtual ~FileManipulatorBase();
		bool Initialize();
		void Finalize();
		/*ファイルの読み込み*/
		std::shared_ptr<File> LoadFile(const std::string& path);
		/*ファイルの保存(保存名、ファイル)*/
		bool SaveFile(const std::string& name, const File& path);
		/*管理情報の更新*/
		bool Reload();
		/*ファイルの存在を確認*/
		bool CheckFileExist(const std::string& path)const;
		/*全てのファイルパスを取得*/
		std::vector<std::string> GetAllFilePaths()const;
		/*有効か*/
		bool is_valid()const { return is_valid_; }
	protected:
		const std::string& root_path()const& { return root_path_; }
		void root_path(const std::string& p) { root_path_ = p; }
		bool is_encrypter_valid()const { return encrypter_ != nullptr; }
		boost::optional<const encrypters::EncrypterBase&> encrypter()const&;
		bool auto_create()const { return auto_create_; }
	private:
		std::string root_path_;
		bool is_valid_; //有効か
		bool auto_create_;
		std::unique_ptr<const encrypters::EncrypterBase> encrypter_;
		virtual bool InitializeProc() = 0;
		virtual void FinalizeProc() = 0;
		virtual bool CheckFileExistenceProc(const std::string& path)const = 0;
		virtual bool ReloadProc() { return true; };
		virtual bool LoadFileProc(const std::string& name, File& file) = 0;
		virtual bool SaveFileProc(const std::string& name, const File& file) = 0;
		virtual size_t GetFileCountProc()const = 0;
		virtual bool GetAllFilePathsProc(std::vector<std::string>& path_list)const = 0;
	};
}
