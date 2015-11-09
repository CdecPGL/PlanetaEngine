#include "NormalFolderLoader.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "SystemLog.h"
#include<fstream>
#include<iomanip>

namespace planeta_engine{
	namespace file_system{
		namespace bfs = boost::filesystem;

		NormalFolderLoader::NormalFolderLoader(const std::string& p) :LoaderBase(p)
		{
		}


		NormalFolderLoader::~NormalFolderLoader()
		{
		}

		bool NormalFolderLoader::_Initialize()
{
			//ファイルリスト取得
			try{
				UpdateFileList();
				debug::SystemLog::instance().LogMessage(std::string("初期化されました。(パス:") + _path + "ファイル数:" + boost::lexical_cast<std::string>(_files.size()) + ")", "NormalFileLoader::_Initialize");
				return true;
			}
			catch (bfs::filesystem_error& e){
				debug::SystemLog::instance().LogError(std::string("初期化に失敗しました。(") + _path + ")", "NormalFileLoader::_Initialize");
				return false;
			}
		}

		void NormalFolderLoader::_Finalize() {

		}

		int NormalFolderLoader::LoadAllFileToCache(){
			int err = 0;
			for (auto& f : _files){
				if (f.second.second->GetStatus() == File::FileStatus::Available){ continue; }
				err = LoadData(f.second.second, f.second.first);
				if (err){
					debug::SystemLog::instance().LogError(std::string("ファイルの読み込みに失敗しました。(") + f.second.first.c_str() + ")", "NormalFileLoader::LoadAllFileToCache");

				}
			}
			return err;
		}

		int NormalFolderLoader::DeleteCache(){
			for (auto& f : _files){
				f.second.second->UnloadData();
			}
			return 0;
		}

		int NormalFolderLoader::UpdateFileList(){
			DeleteCache();
			_files.clear();
			const bfs::path res_path(_path);
			BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(res_path), bfs::recursive_directory_iterator())) {
				if (!bfs::is_directory(p)){
					_files.emplace(std::make_pair(p.filename().string(), std::make_pair(p.string(), std::make_shared<File>(p.extension().string()))));
				}
			}
			return 0;
		}

		std::shared_ptr<File> NormalFolderLoader::LoadFile(const std::string& fn){
			auto it = _files.find(fn);
			if (it == _files.end()){ return nullptr; } //なかったら塗る
			if (it->second.second->GetStatus() == File::FileStatus::Available) { return it->second.second; } //読み込み済みだったら返す
			if (LoadData(it->second.second, it->second.first)){ //読み込み失敗したら塗る
				it->second.second->ErrorOccured();
				return nullptr;
			}
			else{ //読み込んだら返す
				return it->second.second;
			}
		}

		unsigned int NormalFolderLoader::GetCacheSize()const{
			unsigned int res = 0;
			for (auto it : _files){
				res += it.second.second->GetSize();
			}
			return res;
		}

		int NormalFolderLoader::LoadData(const std::shared_ptr<File>& file, const std::string& ap){
			std::ifstream ifs(ap, std::ios::binary | std::ios::in);
			if (!ifs){ return -1; }
			//サイズ取得
			ifs.seekg(0, std::ios::end);
			int size = (int)ifs.tellg();
			//	size -= 4;
			//メモリ確保(Fileに格納し、管理を委ねるのでここでは削除しない)
			unsigned char* ptr = new unsigned char[size];
			if (ptr == nullptr){
				file->ErrorOccured();
				ifs.close();
				return -1;
			}
			//データ初期化
			/*for (int i = 0; i < size; ++i){
				ptr[i] = 0;
				}*/
			//読み込み
			ifs.seekg(0, std::ios::beg);
			ifs.read((char*)ptr, sizeof(unsigned char)*size);

			//デバッガが文字列として読み込めるように
			/*char str[16384]; int ii = 0;
			for (int i = size-16383<0 ? 0:size-16383 ; i < size; ++i,++ii){
			str[ii] = ptr[i];
			}
			str[ii] = '\0';*/

			//データが正常に読み込まれたサイズ計算
			/*int iii = size;
			for (; iii >= 0; --iii){
			if (ptr[iii - 1] != 0){ break; }
			}*/

			//コンソールにデータを出力
			/*for (int i = 0; i < iii+1; ++i){
				if (i % 16 == 0){ printf("%#x : ", i); }
				printf("%02x ", ptr[i]);
				if (i % 16 == 15){ printf("\n"); }
				}
				std::cout << std::endl;*/


			file->SetData(ptr, size);
			//終了
			ifs.close();
			return 0;
		}
	}
}
