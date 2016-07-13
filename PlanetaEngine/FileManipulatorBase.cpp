#include "FileManipulatorBase.h"
#include "SystemLog.h"

namespace planeta {
	bool FileManipulatorBase::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files) {
		bool err = false;
		for (const auto& fn : file_list_) {
			auto file = std::make_shared<File>();
			if (!LoadFileCore(fn, *file)) {
				PE_LOG_ERROR("�t�@�C��", fn, "�̓ǂݍ��݂Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				err = true;
			} else {
				files.push_back(std::make_pair(fn, file));
			}
		}
		return !err;
	}

	bool FileManipulatorBase::CheckFileExist(const std::string& file_name) const {
		return file_list_.find(file_name) != file_list_.end();
	}

	std::shared_ptr<File> FileManipulatorBase::LoadFile(const std::string& name) {
		if (file_list_.find(name) == file_list_.end()) { return nullptr; }
		auto file = std::make_shared<File>();
		if (LoadFileCore(name, *file)) {
			return std::move(file);
		} else {
			return nullptr;
		}
	}

	std::vector<std::pair<std::string, std::shared_ptr<File>>> FileManipulatorBase::LoadAllFiles() {
		UpdateFileList();
		std::vector<std::pair<std::string, std::shared_ptr<File>>> ret;
		if (LoadAllFilesCore(ret)) {
			return std::move(ret);
		} else {
			PE_LOG_ERROR("�t�@�C���̑S���[�h�Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
			return decltype(LoadAllFiles())();
		}
	}

	bool FileManipulatorBase::Initialize() {
		if (is_valid_) { return true; }
		if (InitializeCore()) {
			is_valid_ = true;
			if (UpdateFileList()) {
				PE_LOG_MESSAGE("����������܂����B(�p�X ", path(), ",�t�@�C���� ", file_list_.size(), ",�^�C�v ", GetType().name(), ")");
				return true;
			} else {
				PE_LOG_ERROR("�������Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				return false;
			}
		} else {
			PE_LOG_ERROR("�������Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
			return false;
		}
	}

	bool FileManipulatorBase::SaveFile(const std::string& name, const File& file) {
		if (SaveFileCore(name, file)) {
			file_list_.emplace(name);
			return true;
		} else {
			PE_LOG_ERROR("�t�@�C��,name,�̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
			return false;
		}
	}

	bool FileManipulatorBase::SaveFiles(const std::vector<std::pair<std::string, const File&>>& files) {
		bool ret = SaveFilesCore(files);
		if (!ret) {
			PE_LOG_ERROR("�����t�@�C���̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
		}
		UpdateFileList();
		return ret;
	}

	bool FileManipulatorBase::SaveFilesCore(const std::vector<std::pair<std::string, const File&>>& files) {
		bool err = false;
		for (const auto& f : files) {
			if (!SaveFile(f.first, f.second)) {
				PE_LOG_ERROR("�t�@�C��", f.first, "�̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
				err = true;
			}
		}
		return !err;
	}
}