#pragma once

#include<memory>
#include<string>
#include <unordered_set>
#include "planeta/Object.hpp"
#include"File.hpp"
#include "FileAccessMode.hpp"
#include "boost/optional.hpp"

namespace plnt {
	namespace encrypters {
		class EncrypterBase;
	}
	class FileManipulator : public Object {
	public:
		explicit FileManipulator();
		virtual ~FileManipulator();
		/*マニピュレータを開く*/
		bool Open(const std::string& path, AccessMode access_mode, bool auto_create);
		/*暗号化器を指定してマニピュレータを開く*/
		bool Open(const std::string& path, AccessMode access_mode, std::unique_ptr<const encrypters::EncrypterBase>&& encrypter, bool auto_create);
		/*閉じる*/
		void Close();
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
		/*ファイルアクセスモードを取得する*/
		AccessMode mode()const;
		/*有効か*/
		bool is_opened()const { return is_opened_; }
	protected:
		const std::string& root_path()const& { return root_path_; }
		void root_path(const std::string& p) { root_path_ = p; }
		bool is_encrypter_valid()const { return encrypter_ != nullptr; }
		boost::optional<const encrypters::EncrypterBase&> encrypter()const&;
		bool auto_create()const { return auto_create_; }
	private:
		std::string root_path_;
		bool is_opened_; //有効か
		bool auto_create_;
		AccessMode mode_ = AccessMode::Invalid;
		std::unique_ptr<const encrypters::EncrypterBase> encrypter_;
		virtual bool OpenProc(const std::string& path) = 0;
		virtual void CloseProc() = 0;
		virtual bool CheckFileExistenceProc(const std::string& path)const = 0;
		virtual bool ReloadProc() { return true; };
		virtual bool LoadFileProc(const std::string& name, File& file) = 0;
		virtual bool SaveFileProc(const std::string& name, const File& file) = 0;
		virtual size_t GetFileCountProc()const = 0;
		virtual bool GetAllFilePathsProc(std::vector<std::string>& path_list)const = 0;
	};
}
