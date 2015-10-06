#include "ArchiveLoader.h"
#include"Extracter.h"
#include"boost/filesystem/path.hpp"
#include "SystemLog.h"

namespace planeta_engine{
	namespace file_system{

		ArchiveLoader::ArchiveLoader(const std::string& path) :LoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(0)
		{
		}

		ArchiveLoader::ArchiveLoader(const std::string& path, unsigned int k) : LoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(k)
		{
		}

		ArchiveLoader::~ArchiveLoader()
		{
			_extracter->CloseArchiveFile();
		}

		bool ArchiveLoader::_Initialize()
{
			if (_extracter->SetEXOREncryptionKey((uint16_t)_key)){
				char buf[256];
				sprintf_s(buf, 256, "初期化に失敗しました。復号化キーの設定に失敗しました。(%s)", _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::_Initialize");
				return false;
			}
			int res = _extracter->OpenAchiveFile(_path);
			if (res < 0){
				char buf[256];
				sprintf_s(buf, 256, "初期化に失敗しました。(reason:%d,path:%s)", res, _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::_Initialize");
				return false;
			}
			//ファイルリスト取得
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			char buf[256];
			sprintf_s(buf, 256, "初期化されました。(パス:%s,ファイル数:%d)", _path.c_str(), (int)_files.size());
			debug::SystemLog::instance().LogMessage(buf, "ArchiveLoader::_Initialize");
			return true;
		}

		void ArchiveLoader::_Finalize()
		{

		}

		int ArchiveLoader::LoadAllFileToCache() {
			int err = 0;
			for (auto& f : _files){
				if (f.second->GetStatus() == File::FileStatus::Available){ continue; }
				err = LoadData(f.first, f.second);
				if (err){
					char buf[256];
					sprintf_s(buf, 256, "ファイルの読み込みに失敗しました。(name:%s)", f.first.c_str());
					debug::SystemLog::instance().LogError(buf, "ArchiveLoader::LoadAllFileToCatch");
				}
			}
			return err;
		}

		int ArchiveLoader::DeleteCache(){
			for (auto& f : _files){
				f.second->UnloadData();
			}
			return 0;
		}

		int ArchiveLoader::UpdateFileList(){
			DeleteCache();
			_extracter->CloseArchiveFile();
			_files.clear();
			int res = _extracter->OpenAchiveFile(_path);
			if (res < 0){
				char buf[256];
				sprintf_s(buf, 256, "アーカイブファイルのオープンに失敗しました。(reason:%d,path:%s)", res, _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::LoadAllFileToCatch");
			}
			//ファイルリスト取得
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			return 0;
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
			file->SetData(data.first, data.second);
			return 0;
		}

		unsigned int ArchiveLoader::GetCacheSize()const{
			unsigned int res = 0;
			for (auto& it : _files){
				res += it.second->GetSize();
			}
			return res;
		}

		void ArchiveLoader::SetKey(unsigned int k){ _key = k; }
	}
}
