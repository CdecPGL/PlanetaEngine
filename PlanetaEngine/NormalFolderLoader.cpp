#include "NormalFolderLoader.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include "SystemLog.h"
#include "EncrypterBase.h"
#include<fstream>
#include<iomanip>

namespace planeta_engine{
	namespace file_system{
		namespace bfs = boost::filesystem;
		NormalFolderLoader::~NormalFolderLoader()
		{
		}

		bool NormalFolderLoader::_Initialize()
{
			//ファイルリスト取得
			try{
				UpdateFileList();
				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "初期化されました。(パス ", path(), ",ファイル数 ", files_.size(), ")");
				return true;
			}
			catch (bfs::filesystem_error& e){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。(パス ", path(), ")");
				return false;
			}
		}

		void NormalFolderLoader::_Finalize() {

		}

		bool NormalFolderLoader::LoadAllFileToCache(){
			bool err = false;
			for (auto& f : files_){
				if (f.second.second->GetStatus() == File::FileStatus::Available){ continue; }
				if (!LoadFile(f.second.second, f.second.first)){
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", f.second.first, "の読み込みに失敗しました。");
					err = true;
				}
			}
			return err;
		}

		bool NormalFolderLoader::DeleteCache(){
			for (auto& f : files_){
				f.second.second->UnloadData();
			}
			return true;
		}

		bool NormalFolderLoader::UpdateFileList(){
			DeleteCache();
			files_.clear();
			const bfs::path res_path(path());
			BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(res_path), bfs::recursive_directory_iterator())) {
				if (!bfs::is_directory(p)){
					files_.emplace(std::make_pair(p.filename().string(), std::make_pair(p.string(), std::make_shared<File>(p.extension().string()))));
				}
			}
			return true;
		}

		std::shared_ptr<File> NormalFolderLoader::LoadFile(const std::string& fn){
			auto it = files_.find(fn);
			if (it == files_.end()){ return nullptr; } //なかったら塗る
			if (it->second.second->GetStatus() == File::FileStatus::Available) { return it->second.second; } //読み込み済みだったら返す
			if (!LoadFile(it->second.second, it->second.first)){ //読み込み失敗したら塗る
				it->second.second->ErrorOccured();
				return nullptr;
			}
			else{ //読み込んだら返す
				return it->second.second;
			}
		}

		unsigned int NormalFolderLoader::GetCacheSize()const{
			unsigned int res = 0;
			for (auto it : files_){
				res += it.second.second->GetSize();
			}
			return res;
		}

		bool NormalFolderLoader::LoadFile(const std::shared_ptr<File>& file, const std::string& name)
		{
			if (LoadDataCore(file, name) < 0) { return false; }
			else {
				if (is_encrypter_valid()) {
					if (!encrypter()->Decrypt(*file)) { return true; }
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

		int NormalFolderLoader::LoadDataCore(const std::shared_ptr<File>& file, const std::string& ap) {
			std::ifstream ifs(ap, std::ios::binary | std::ios::in);
			if (!ifs){ return -1; }
			//サイズ取得
			ifs.seekg(0, std::ios::end);
			int size = (int)ifs.tellg();
			//メモリ確保
			if (!file->ChangeSize(size)){
				ifs.close();
				return -1;
			}
			ifs.seekg(0, std::ios::beg);
			ifs.read(reinterpret_cast<char*>(file->GetTopPointer()), file->GetSize());
			//終了
			ifs.close();
			return 0;
		}
	}
}
