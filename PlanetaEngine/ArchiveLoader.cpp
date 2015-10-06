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
				sprintf_s(buf, 256, "�������Ɏ��s���܂����B�������L�[�̐ݒ�Ɏ��s���܂����B(%s)", _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::_Initialize");
				return false;
			}
			int res = _extracter->OpenAchiveFile(_path);
			if (res < 0){
				char buf[256];
				sprintf_s(buf, 256, "�������Ɏ��s���܂����B(reason:%d,path:%s)", res, _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::_Initialize");
				return false;
			}
			//�t�@�C�����X�g�擾
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			char buf[256];
			sprintf_s(buf, 256, "����������܂����B(�p�X:%s,�t�@�C����:%d)", _path.c_str(), (int)_files.size());
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
					sprintf_s(buf, 256, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(name:%s)", f.first.c_str());
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
				sprintf_s(buf, 256, "�A�[�J�C�u�t�@�C���̃I�[�v���Ɏ��s���܂����B(reason:%d,path:%s)", res, _path.c_str());
				debug::SystemLog::instance().LogError(buf, "ArchiveLoader::LoadAllFileToCatch");
			}
			//�t�@�C�����X�g�擾
			std::vector<std::string> f_list = std::move(_extracter->GetFileList());
			for (auto& fn : f_list){
				boost::filesystem::path p(fn);
				_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
			}
			return 0;
		}

		std::shared_ptr<File> ArchiveLoader::LoadFile(const std::string& fn){
			auto it = _files.find(fn);
			if (it == _files.end()){ return nullptr; } //�Ȃ�������h��
			if ((*it).second->GetStatus() == File::FileStatus::Available){ return (*it).second; } //�ǂݍ��ݍς݂�������Ԃ�
			if (LoadData(fn, (*it).second)){ //�ǂݍ��ݎ��s������h��
				it->second->ErrorOccured();
				return nullptr;
			}
			else{ //�ǂݍ��񂾂�Ԃ�
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
