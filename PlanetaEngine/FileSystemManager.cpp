#include "FileSystemManager.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include <cassert>

namespace planeta {
	FileSystemManager::FileSystemManager() {

	}


	FileSystemManager::~FileSystemManager() {

	}

	bool FileSystemManager::Initialize() {
		bool error = false;
		if (error) {
			debug::SystemLog::instance().LogError("�������Ɏ��s���܂����BFileAccessor�̏������Ɏ��s���܂����B", __FUNCTION__);
		}
		if (!error) { is_initialized_ = true; }
		return !error;
	}

	void FileSystemManager::Finalize() {
		for (auto& accessor : accessors_) {
			accessor.second->Finalize();
		}
		is_initialized_ = false;
	}

	size_t FileSystemManager::GetCacheSize()const {
		size_t res = 0;
		for (auto& accessor : accessors_) {
			res += accessor.second->GetCacheSize();
		}
		return res;
	}

	bool FileSystemManager::DeleteCache() {
		for (auto& accessor : accessors_) {
			accessor.second->DeleteCache();
		}
		debug::SystemLog::instance().LogMessage("�t�@�C���L���b�V�����폜���܂����B", __FUNCTION__);
		return true;
	}

	std::shared_ptr<FileAccessor> FileSystemManager::CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
		auto accessor = CreateFileAccessorCore(id, manipulator, mode);
		if (accessor->Initialize()) {
			accessors_.emplace(id, accessor);
			return accessor;
		} else {
			PE_LOG_ERROR("�t�@�C���A�N�Z�T�̏������Ɏ��s���܂����B");
			return nullptr;
		}
	}

	std::shared_ptr<FileAccessor> FileSystemManager::GetFileAccessor(const std::string& id) const {
		auto it = accessors_.find(id);
		if (it == accessors_.end()) { return nullptr; } else {
			return it->second;
		}
	}

	std::shared_ptr<FileAccessor> FileSystemManager::CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
		assert(is_initialized_ == true); //����͏�������ɌĂ�
		auto accessor = std::make_shared<FileAccessor>(manipulator, mode);
		return std::move(accessor);
	}

	bool FileSystemManager::DisposeFileAccessor(const std::string& id) {
		auto it = accessors_.find(id);
		if (it == accessors_.end()) { 
			PE_LOG_ERROR("�w�肳�ꂽ�t�@�C���A�N�Z�T\"", id, "\"�͑��݂��Ȃ����ߔj���ł��܂���ł����B");
			return false; 
		} 
		else {
			it->second->Finalize();
			accessors_.erase(it);
			PE_LOG_MESSAGE("�t�@�C���A�N�Z�T\"", id, "\"��j�����܂����B");
			return true;
		}
	}

}
