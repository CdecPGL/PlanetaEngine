#include "FileLoadManager.h"

namespace PlanetaEngine{
	namespace FileSystem{

		FileLoadManager::FileLoadManager()
		{
			printf("FileLoadManager�̃C���X�^���X���쐬����܂����B\n");
		}


		FileLoadManager::~FileLoadManager()
		{
			for (auto it : _loaders){
				delete it;
			}
			printf("FileLoadManager�̃C���X�^���X���j������܂����B\n");
		}

		FileLoadManager& FileLoadManager::GetInstance(){
			static FileLoadManager flm;
			return flm;
		}

		int FileLoadManager::Init(){
			for (auto it : _loaders){
				if (it->Init()){
					printf("FileLoadManager�̏������Ɏ��s���܂����B\n");
					return -1;
				}
			}
			printf("FileLoadManager������������܂����B\n");
			return 0;
		}

		void FileLoadManager::PushLoader(LoaderBase* lb){
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
				printf("FileLoadManager::FindFile �t�@�C�������݂��Ȃ����ǂݍ��݃G���[���������܂����B(%s)\n", fn.c_str());
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
			return err;
		}

		int FileLoadManager::DeleteCache(){
			for (auto it : _loaders){
				it->DeleteCache();
			}
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
