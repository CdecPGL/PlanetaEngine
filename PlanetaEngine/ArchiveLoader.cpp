#include "ArchiveLoader.h"
#include"Extracter.h"
#include"boost/filesystem/path.hpp"
#include "SystemLog.h"

namespace planeta_engine{
	namespace file_system{

		ArchiveLoader::ArchiveLoader(const std::string& path) :FileLoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(0)
		{
		}

		ArchiveLoader::ArchiveLoader(const std::string& path, unsigned int k) : FileLoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(k)
		{
		}

		ArchiveLoader::~ArchiveLoader()
		{
			_extracter->CloseArchiveFile();
		}

		bool ArchiveLoader::_Initialize()
{
			if (_extracter->SetEXOREncryptionKey((uint16_t)_key)){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。復号化キーの設定に失敗しました。(パス ", path(), ")");
				return false;
			}
			int res = _extracter->OpenAchiveFile(path());
			if (res < 0){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。(原因 ", res, ",パス ", path(), ")");
				return false;
			}
			//ファイルリスト取得
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "初期化されました。(パス ", path(), ",ファイル数 ", _files.size(), ")");
			return true;
		}

		void ArchiveLoader::_Finalize()
		{

		}

		bool ArchiveLoader::LoadAllFileToCache() {
			int err = 0;
			for (auto& f : _files){
				if (f.second->GetStatus() == File::FileStatus::Available){ continue; }
				err = LoadData(f.first, f.second);
				if (err){
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", f.first, "の読み込みに失敗しました。");
				}
			}
			return err ? false : true;
		}

		bool ArchiveLoader::DeleteCache(){
			for (auto& f : _files){
				f.second->UnloadData();
			}
			return true;
		}

		bool ArchiveLoader::UpdateFileList(){
			DeleteCache();
			_extracter->CloseArchiveFile();
			_files.clear();
			int res = _extracter->OpenAchiveFile(path());
			if (res < 0){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "アーカイブファイルのオープンに失敗しました。(原因 ", res, ",パス ", path(), ")");
				return false;
			}
			//ファイルリスト取得
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			return true;
		}

		std::shared_ptr<File> ArchiveLoader::LoadFile(const std::string& fn){
			auto it = _files.find(fn);
			if (it == _files.end()){ return nullptr; } //なかったら塗る
			if ((*it).second->GetStatus() == File::FileStatus::Available){ return (*it).second; } //読み込み済みだったら返す
			if (LoadData(fn, (*it).second)){ //読み込み失敗したら塗る
				it->second->ErrorOccured();
				return nullptr;
			}
			else{ //読み込んだら返す
				return it->second;
			}
		}

		int ArchiveLoader::LoadData(const std::string& fn, const std::shared_ptr<File>& file){
			auto data = _extracter->GetFile(fn);
			if (data.first == nullptr){
				return -1;
			}
			file->SetData(reinterpret_cast<unsigned char*>(data.first), data.second);
			return 0;
		}

		size_t ArchiveLoader::GetCacheSize()const{
			unsigned int res = 0;
			for (auto& it : _files){
				res += it.second->GetSize();
			}
			return res;
		}

		void ArchiveLoader::SetKey(unsigned int k){ _key = k; }
	}
}
