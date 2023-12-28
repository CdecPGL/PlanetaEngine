#pragma once

#include<memory>
#include<string>

#include "Object.hpp"
#include"File.hpp"
#include "FileAccessMode.hpp"

namespace plnt {
	namespace archiver {
		class encrypter_base;
	}

	class file_manipulator : public Object {
	public:
		explicit file_manipulator();
		file_manipulator(const file_manipulator &) = delete;
		file_manipulator(file_manipulator &&) = delete;
		~file_manipulator() override;
		file_manipulator &operator =(const file_manipulator &) = delete;
		file_manipulator &operator =(file_manipulator &&) = delete;

		/*マニピュレータを開く*/
		bool open(const std::string &path, AccessMode access_mode, bool auto_create);
		/*暗号化器を指定してマニピュレータを開く*/
		bool open(const std::string &path, AccessMode access_mode,
		          std::unique_ptr<const archiver::encrypter_base> &&encrypter, bool auto_create);
		/*閉じる*/
		void close();
		/*ファイルの読み込み*/
		std::shared_ptr<File> load_file(const std::string &name);
		/*ファイルの保存(保存名、ファイル)*/
		bool save_file(const std::string &name, const File &file);
		/*管理情報の更新*/
		bool reload();
		/*ファイルの存在を確認*/
		[[nodiscard]] bool check_file_exist(const std::string &path) const;
		/*全てのファイルパスを取得*/
		[[nodiscard]] std::vector<std::string> get_all_file_paths() const;
		/*ファイルアクセスモードを取得する*/
		[[nodiscard]] AccessMode mode() const;
		/*有効か*/
		[[nodiscard]] bool is_opened() const { return is_opened_; }

	protected:
		[[nodiscard]] const std::string &root_path() const & { return root_path_; }
		void root_path(const std::string &p) { root_path_ = p; }
		[[nodiscard]] bool is_encrypter_valid() const { return encrypter_ != nullptr; }
		[[nodiscard]] boost::optional<const archiver::encrypter_base &> encrypter() const &;
		[[nodiscard]] bool auto_create() const { return auto_create_; }

	private:
		std::string root_path_;
		bool is_opened_; //有効か
		bool auto_create_;
		AccessMode mode_ = AccessMode::Invalid;
		std::unique_ptr<const archiver::encrypter_base> encrypter_;
		virtual bool open_proc(const std::string &path) = 0;
		virtual void close_proc() = 0;
		[[nodiscard]] virtual bool check_file_existence_proc(const std::string &path) const = 0;
		virtual bool reload_proc() { return true; };
		virtual bool load_file_proc(const std::string &name, File &file) = 0;
		virtual bool save_file_proc(const std::string &name, const File &file) = 0;
		[[nodiscard]] virtual size_t get_file_count_proc() const = 0;
		virtual bool get_all_file_paths_proc(std::vector<std::string> &path_list) const = 0;
	};
}
