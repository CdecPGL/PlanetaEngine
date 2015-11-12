#include "NormalFolderManipulator.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include "SystemLog.h"
#include "EncrypterBase.h"
#include<fstream>
#include<iomanip>

namespace planeta_engine{
	namespace file_system{
		namespace bfs = boost::filesystem;
		NormalFolderManipulator::~NormalFolderManipulator()
		{
		}

		bool NormalFolderManipulator::InitializeCore(){
			return true;
		}

		void NormalFolderManipulator::FinalizeCore() {

		}

		bool NormalFolderManipulator::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files){
			bool err = false;
			for (auto& f : file_name_path_map_){
				auto file = std::make_shared<File>();
				if (!LoadFileByPath(*file,f.second)){
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", f.first, "の読み込みに失敗しました。パスは", f.second, "です。");
					err = true;
				}
				else {
					files.push_back(std::make_pair(f.first, std::move(file)));
				}
			}
			return !err;
		}

		bool NormalFolderManipulator::SaveFileCore(const std::string& name, const File& file)
		{
			std::ofstream ofs(path() + "\\" + name, std::ios::binary | std::ios::trunc);
			if (!ofs) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", name, "をディレクトリ", path(), "に保存できませんでした");
				return false;
			}
			if (is_encrypter_valid()) { //暗号化が有効だったら
				File encd_file;
				if (!encrypter()->Encrypt(file, encd_file)) { //暗号化して
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "暗号化に失敗しました。");
					return false;
				}
				ofs.write(reinterpret_cast<char*>(encd_file.GetTopPointer()), encd_file.GetSize()); //保存
			}
			else {
				ofs.write(reinterpret_cast<const char*>(file.GetTopPointer()), file.GetSize());
			}
			return true;
		}

		bool NormalFolderManipulator::UpdateFileListCore(std::unordered_set<std::string>& file_list) {
			try {
				file_name_path_map_.clear();
				const bfs::path res_path(path());
				BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(res_path), bfs::recursive_directory_iterator())) {
					if (!bfs::is_directory(p)) {
						file_list.emplace(p.filename().string());
						file_name_path_map_.emplace(p.filename().string(), p.string());
					}
				}
				return true;
			}
			catch (bfs::filesystem_error& e) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイルリストの更新に失敗しました。(パス ", path(), ")");
				return false;
			}
		}

		bool NormalFolderManipulator::LoadFileCore(const std::string& fn,File& file){
			auto it = file_name_path_map_.find(fn);
			assert(it != file_name_path_map_.end()); //存在チェックはファイルリストを参考にすでに行われているはず。
			return LoadFileByPath(file, it->second);
		}

		bool NormalFolderManipulator::LoadFileByPath(File& file, const std::string& name)
		{
			if (LoadDataCore(file, name) < 0) { return false; }
			else {
				if (is_encrypter_valid()) {
					if (!encrypter()->Decrypt(file)) { return true; }
					else { 
						debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "復号化に失敗しました。");
						return false;
					}
				}
				else {
					return true;
				}
			}
		}

		int NormalFolderManipulator::LoadDataCore(File& file, const std::string& ap) {
			std::ifstream ifs(ap, std::ios::binary | std::ios::in);
			if (!ifs){ return -1; }
			//サイズ取得
			ifs.seekg(0, std::ios::end);
			int size = (int)ifs.tellg();
			//メモリ確保
			if (!file.ChangeSize(size)){
				ifs.close();
				return -1;
			}
			ifs.seekg(0, std::ios::beg);
			ifs.read(reinterpret_cast<char*>(file.GetTopPointer()), file.GetSize());
			//終了
			ifs.close();
			return 0;
		}
	}
}
