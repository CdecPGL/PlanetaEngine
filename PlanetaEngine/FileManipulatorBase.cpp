#include "FileManipulatorBase.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {

		bool FileManipulatorBase::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files)
		{
			bool err = false;
			for (const auto& fn : file_list_) {
				auto file = std::make_shared<File>();
				if (!LoadFileCore(fn, *file)) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", fn, "�̓ǂݍ��݂Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
					err = true;
				}
				else {
					files.push_back(std::make_pair(fn, file));
				}
			}
			return !err;
		}

		bool FileManipulatorBase::CheckFileExist(const std::string& file_name) const
		{
			return file_list_.find(file_name) != file_list_.end();
		}

		std::shared_ptr<File> FileManipulatorBase::LoadFile(const std::string& name)
		{
			if (file_list_.find(name) == file_list_.end()) { return nullptr; }
			auto file = std::make_shared<File>();
			if (LoadFileCore(name, *file)) {
				return std::move(file);
			}
			else {
				return nullptr;
			}
		}

		std::vector<std::pair<std::string, std::shared_ptr<File>>> FileManipulatorBase::LoadAllFiles()
		{
			UpdateFileList();
			std::vector<std::pair<std::string, std::shared_ptr<File>>> ret;
			if (LoadAllFilesCore(ret)) {
				return std::move(ret);
			}
			else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C���̑S���[�h�Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				return decltype(LoadAllFiles())();
			}
		}

		bool FileManipulatorBase::Initialize()
		{
			if (is_valid_) { return true; }
			if (InitializeCore()) { 
				is_valid_ = true; 
				if (UpdateFileList()) {
					debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "����������܂����B(�p�X ", path(), ",�t�@�C���� ", file_list_.size(), ",�^�C�v ", GetType().name(), ")");
					return true;
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
					return false;
				}
			}
			else { 
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				return false;
			}
		}

		bool FileManipulatorBase::SaveFile(const std::string& name, const File& file)
		{
			if (SaveFileCore(name, file)) {
				file_list_.emplace(name);
				return true;
			}
			else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��,name,�̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				return false;
			}
		}

		bool FileManipulatorBase::SaveFiles(const std::vector<std::pair<std::string, const File&>>& files)
		{
			bool ret = SaveFilesCore(files);
			if (!ret) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�����t�@�C���̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
			}
			UpdateFileList();
			return ret;
		}

		bool FileManipulatorBase::SaveFilesCore(const std::vector<std::pair<std::string, const File&>>& files)
		{
			bool err;
			for (const auto& f : files) {
				if (!SaveFile(f.first, f.second)) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", f.first, "�̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
					err = true;
				}
			}
			return !err;
		}

	}
}