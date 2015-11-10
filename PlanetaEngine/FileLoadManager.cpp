#include "FileLoadManager.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace file_system{

		FileLoadManager::FileLoadManager()
		{
			
		}


		FileLoadManager::~FileLoadManager()
		{
			
		}

		bool FileLoadManager::Initialize(){
			bool error_flag = false;
			bool success_flag = false;
			for (auto it : _loaders){
				if (it->Initialize()){
					success_flag = true;
				}
				else {
					error_flag = true;
				}
			}
			if (success_flag && error_flag) {
				debug::SystemLog::instance().LogWarning("一部ローダーの初期化に失敗しましたが、継続します。", __FUNCTION__);
			}
			else if(success_flag && !error_flag) {
				debug::SystemLog::instance().LogMessage("初期化に成功しました。", __FUNCTION__);
			}
			else {
				debug::SystemLog::instance().LogError("初期化に失敗しました。", __FUNCTION__);
			}
			return true;
		}

		bool FileLoadManager::Finalize() {
			_loaders.clear();
			return true;
		}

		void FileLoadManager::PushLoader(const std::shared_ptr<FileLoaderBase>& lb){
			_loaders.push_back(lb);
		}

		std::shared_ptr<File> FileLoadManager::LoadFile(const std::string& fn){
			std::shared_ptr<File> f;
			//末尾からローダーを試し、成功したら終了
			for (auto it = _loaders.rbegin(); it != _loaders.rend(); ++it){
				f = (*it)->LoadFile(fn);
				if (f){ break; }
			}
			if (f == nullptr){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", fn, "が存在しないか読み込みエラーが発生しました。");
			}
			return f;
		}

		unsigned int FileLoadManager::GetCacheSize()const{
			unsigned int res = 0;
			for (auto it : _loaders){
				res += it->GetCacheSize();
			}
			return res;
		}

		int FileLoadManager::LoadAllFileToCache(){
			int err = 0;
			for (auto it : _loaders){
				err = it->LoadAllFileToCache();
			}
			debug::SystemLog::instance().LogMessage("すべてのファイルをキャッシュに読み込みました。", __FUNCTION__);
			return err;
		}

		int FileLoadManager::DeleteCache(){
			for (auto it : _loaders){
				it->DeleteCache();
			}
			debug::SystemLog::instance().LogMessage("ファイルキャッシュを削除しました。", __FUNCTION__);
			return 0;
		}

		int FileLoadManager::UpdateFileList(){
			for (auto it : _loaders){
				it->UpdateFileList();
			}
			return 0;
		}
	}
}
