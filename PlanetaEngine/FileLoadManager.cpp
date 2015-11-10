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
				debug::SystemLog::instance().LogWarning("�ꕔ���[�_�[�̏������Ɏ��s���܂������A�p�����܂��B", __FUNCTION__);
			}
			else if(success_flag && !error_flag) {
				debug::SystemLog::instance().LogMessage("�������ɐ������܂����B", __FUNCTION__);
			}
			else {
				debug::SystemLog::instance().LogError("�������Ɏ��s���܂����B", __FUNCTION__);
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
			//�������烍�[�_�[�������A����������I��
			for (auto it = _loaders.rbegin(); it != _loaders.rend(); ++it){
				f = (*it)->LoadFile(fn);
				if (f){ break; }
			}
			if (f == nullptr){
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", fn, "�����݂��Ȃ����ǂݍ��݃G���[���������܂����B");
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
			debug::SystemLog::instance().LogMessage("���ׂẴt�@�C�����L���b�V���ɓǂݍ��݂܂����B", __FUNCTION__);
			return err;
		}

		int FileLoadManager::DeleteCache(){
			for (auto it : _loaders){
				it->DeleteCache();
			}
			debug::SystemLog::instance().LogMessage("�t�@�C���L���b�V�����폜���܂����B", __FUNCTION__);
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
