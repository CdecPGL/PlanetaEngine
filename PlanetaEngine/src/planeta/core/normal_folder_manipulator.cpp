#include<fstream>
#include<iomanip>

#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"

#include "normal_folder_manipulator.hpp"
#include "LogUtility.hpp"
#include "../archiver/encrypter_base.hpp"

namespace plnt {
	namespace bfs = boost::filesystem;

	bool normal_folder_manipulator::open_proc(const std::string &path) {
		//ディレクトリの存在確認
		const boost::filesystem::path b_path(path);
		if (is_directory(b_path)) { return true; }
		//ディレクトリがなくて自動作成が有効だったら作成する。
		if (auto_create()) {
			if (create_directories(b_path)) {
				PE_LOG_MESSAGE("ディレクトリ", path, "を作成しました。");
				return true;
			}
			PE_LOG_ERROR("初期化に失敗しました。ディレクトリ", path, "を作成できませんでした。");
			return false;
		}
		PE_LOG_ERROR("初期化に失敗しました。ディレクトリ", path, "が存在しません。");
		return false;
	}

	void normal_folder_manipulator::close_proc() { }

	bool normal_folder_manipulator::save_file_proc(const std::string &name, const file &f) {
		auto file_path = root_path() + "\\" + name;
		std::ofstream ofs(file_path, std::ios::binary | std::ios::trunc);
		if (!ofs) {
			PE_LOG_ERROR("ファイル", file_path, "をディレクトリ", root_path(), "に保存できませんでした");
			return false;
		}
		if (is_encrypter_valid()) {
			//暗号化が有効だったら
			file encrypted_file;
			if (!encrypter()->encrypt(f, encrypted_file)) {
				//暗号化して
				PE_LOG_ERROR("暗号化に失敗しました。");
				return false;
			}
			ofs.write(reinterpret_cast<char *>(encrypted_file.top_pointer()), encrypted_file.size()); //保存
		} else { ofs.write(reinterpret_cast<const char *>(f.top_pointer()), f.size()); }
		return true;
	}

	bool normal_folder_manipulator::load_file_proc(const std::string &fn, file &file) {
		return load_file_by_path(file, fn);
	}

	bool normal_folder_manipulator::load_file_by_path(file &file, const std::string &name) const {
		if (auto file_path = root_path() + "\\" + name; load_data_core(file, file_path) < 0) {
			PE_LOG_ERROR("ファイル\"", file_path, "\"の読み込みに失敗しました。ファイルが存在しない可能性があります。");
			return false;
		}
		if (is_encrypter_valid()) {
			if (encrypter()->decrypt(file)) {
				PE_LOG_ERROR("復号化に失敗しました。");
				return false;
			}
			return true;
		}
		return true;
	}

	int normal_folder_manipulator::load_data_core(file &file, const std::string &ap) {
		std::ifstream ifs(ap, std::ios::binary | std::ios::in);
		if (!ifs) { return -1; }
		//サイズ取得
		ifs.seekg(0, std::ios::end);
		//メモリ確保
		if (const auto size = ifs.tellg(); !file.reserve(size)) {
			ifs.close();
			return -1;
		}
		ifs.seekg(0, std::ios::beg);
		ifs.read(reinterpret_cast<char *>(file.top_pointer()), file.size());
		//終了
		ifs.close();
		return 0;
	}

	size_t normal_folder_manipulator::get_file_count_proc() const {
		size_t cnt = 0;
		const bfs::path path(root_path());
		BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(path),
			              bfs::recursive_directory_iterator())) { if (!is_directory(p)) { ++cnt; } }
		return cnt;
	}

	bool normal_folder_manipulator::check_file_existence_proc(const std::string &p_path) const {
		const bfs::path path(root_path() + "\\" + p_path);

		boost::system::error_code error;
		if (const bool result = exists(path, error); !result || error) { return false; }
		return true;
	}

	bool normal_folder_manipulator::get_all_file_paths_proc(std::vector<std::string> &path_list) const {
		const bfs::path path(root_path());
		BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(path),
			              bfs::recursive_directory_iterator())) {
			if (!is_directory(p)) {
				//カレントディレクトリからの相対パスを取得
				//auto rp_str = p.relative_path().string();
				auto rp_str = p.string();
				//root_path()からの相対パスに変換する
				path_list.push_back(rp_str.substr(root_path().size() + 1, rp_str.size() - 1));
			}
		}
		return true;
	}
}
