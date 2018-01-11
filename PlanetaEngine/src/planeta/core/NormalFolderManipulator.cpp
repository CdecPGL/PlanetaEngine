#include "planeta/core/NormalFolderManipulator.hpp"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include "planeta/LogUtility.hpp"
#include "planeta/archiver/EncrypterBase.hpp"
#include<fstream>
#include<iomanip>

namespace plnt {
	namespace bfs = boost::filesystem;
	NormalFolderManipulator::~NormalFolderManipulator() {}

	bool NormalFolderManipulator::OpenProc(const std::string& path) {
		//ディレクトリの存在確認
		boost::filesystem::path bpath(path);
		if (boost::filesystem::is_directory(bpath)) {
			return true;
		} else {
			//ディレクトリがなくて自動作成が有効だったら作成する。
			if (auto_create()) {
				if (boost::filesystem::create_directories(bpath)) {
					PE_LOG_MESSAGE("ディレクトリ", path, "を作成しました。");
					return true;
				} else {
					PE_LOG_ERROR("初期化に失敗しました。ディレクトリ", path, "を作成できませんでした。");
					return false;
				}
			} else {
				PE_LOG_ERROR("初期化に失敗しました。ディレクトリ", path, "が存在しません。");
				return false;
			}
		}
	}

	void NormalFolderManipulator::CloseProc() {}

	bool NormalFolderManipulator::SaveFileProc(const std::string& name, const File& file) {
		auto file_path = root_path() + "\\" + name;
		std::ofstream ofs(file_path, std::ios::binary | std::ios::trunc);
		if (!ofs) {
			PE_LOG_ERROR("ファイル", file_path, "をディレクトリ", root_path(), "に保存できませんでした");
			return false;
		}
		if (is_encrypter_valid()) { //暗号化が有効だったら
			File encd_file;
			if (!encrypter()->Encrypt(file, encd_file)) { //暗号化して
				PE_LOG_ERROR("暗号化に失敗しました。");
				return false;
			}
			ofs.write(reinterpret_cast<char*>(encd_file.top_pointer()), encd_file.size()); //保存
		} else {
			ofs.write(reinterpret_cast<const char*>(file.top_pointer()), file.size());
		}
		return true;
	}

	bool NormalFolderManipulator::LoadFileProc(const std::string& fn, File& file) {
		return LoadFileByPath(file, fn);
	}

	bool NormalFolderManipulator::LoadFileByPath(File& file, const std::string& name) {
		auto file_path = root_path() + "\\" + name;
		if (LoadDataCore(file, file_path) < 0) {
			PE_LOG_ERROR("ファイル\"", file_path, "\"の読み込みに失敗しました。ファイルが存在しない可能性があります。");
			return false;
		} else {
			if (is_encrypter_valid()) {
				if (!encrypter()->Decrypt(file)) { return true; } else {
					PE_LOG_ERROR("復号化に失敗しました。");
					return false;
				}
			} else {
				return true;
			}
		}
	}

	int NormalFolderManipulator::LoadDataCore(File& file, const std::string& ap) {
		std::ifstream ifs(ap, std::ios::binary | std::ios::in);
		if (!ifs) {
			return -1;
		}
		//サイズ取得
		ifs.seekg(0, std::ios::end);
		int size = (int)ifs.tellg();
		//メモリ確保
		if (!file.Reserve(size)) {
			ifs.close();
			return -1;
		}
		ifs.seekg(0, std::ios::beg);
		ifs.read(reinterpret_cast<char*>(file.top_pointer()), file.size());
		//終了
		ifs.close();
		return 0;
	}

	size_t NormalFolderManipulator::GetFileCountProc() const {
		size_t cnt = 0;
		const bfs::path path(root_path());
		BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(path),
			bfs::recursive_directory_iterator())) {
			if (!bfs::is_directory(p)) { ++cnt; }
		}
		return cnt;
	}

	bool NormalFolderManipulator::CheckFileExistenceProc(const std::string& ppath) const {
		const bfs::path path(root_path() + "\\" + ppath);

		boost::system::error_code error;
		const bool result = bfs::exists(path, error);
		if (!result || error) {
			return false;
		} else {
			return true;
		}
	}

	bool NormalFolderManipulator::GetAllFilePathsProc(std::vector<std::string>& path_list) const {
		const bfs::path path(root_path());
		BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(path),
			bfs::recursive_directory_iterator())) {
			if (!bfs::is_directory(p)) {
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
