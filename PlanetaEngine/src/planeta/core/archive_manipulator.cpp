#include "archive_manipulator.hpp"
// TODO: 必要か調べる。必要なら探す
//#include"Extracter.h"
//#include "Archiver.h"
#include"boost/filesystem/path.hpp"
#include "log_utility.hpp"

namespace plnt {
	archive_manipulator::archive_manipulator() : file_manipulator()/*, _extracter(std::make_unique<Extracter>())*/,
	                                             key_(0) {}

	archive_manipulator::~archive_manipulator() {
		//_extracter->CloseArchiveFile();
	}

	bool archive_manipulator::open_proc([[maybe_unused]] const std::string &path) {
		/*if (_extracter->SetEXOREncryptionKey((uint16_t)_key)) {
			PE_LOG_ERROR("初期化に失敗しました。復号化キーの設定に失敗しました。(パス ", path(), ")");
			return false;
		}
		int res = _extracter->OpenAchiveFile(root_path());
		if (res < 0) {
			PE_LOG_ERROR("初期化に失敗しました。アーカイブが開けませんでした。(原因 ", res, ",パス ", path(), ")");
			return false;
		}
		return true;*/
		return false;
	}

	void archive_manipulator::close_proc() {}

	bool archive_manipulator::reload_proc() {
		//_extracter->CloseArchiveFile();
		//int res = _extracter->OpenAchiveFile(root_path());
		//if (res < 0) {
		//	PE_LOG_ERROR("アーカイブファイルのオープンに失敗しました。(原因 ", res, ",パス ", path(), ")");
		//	return false;
		//}
		////ファイルリスト取得
		//std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		//for (auto& fn : f_list) {
		//	boost::filesystem::path p(fn);
		//	file_list.emplace(fn);
		//}
		//return true;
		return false;
	}

	bool archive_manipulator::load_file_proc([[maybe_unused]] const std::string &fn, [[maybe_unused]] file &file) {
		/*auto data = _extracter->GetFile(fn);
		if (data.first == nullptr) {
			return false;
		}
		file.MoveData(reinterpret_cast<unsigned char*>(data.first), data.second);
		return true;*/
		return false;
	}

	void archive_manipulator::set_key(const unsigned int k) { key_ = k; }

	bool archive_manipulator::check_file_existence_proc([[maybe_unused]] const std::string &path) const {
		return false;
	}

	size_t archive_manipulator::get_file_count_proc() const { return false; }

	bool archive_manipulator::get_all_file_paths_proc([[maybe_unused]] std::vector<std::string> &path_list) const {
		return false;
	}
}
