#include "FileSystemManager.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include <cassert>

namespace planeta_engine {
	FileSystemManager::FileSystemManager() {

	}


	FileSystemManager::~FileSystemManager() {

	}

	bool FileSystemManager::Initialize() {
		assert(is_initialized_ == false);
		bool error = false;
		for (auto& accessor : accessors_) {
			error = !accessor.second->Initialize();
		}
		if (error) {
			debug::SystemLog::instance().LogError("初期化に失敗しました。FileAccessorの初期化に失敗しました。", __FUNCTION__);
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
		debug::SystemLog::instance().LogMessage("ファイルキャッシュを削除しました。", __FUNCTION__);
		return true;
	}

	std::shared_ptr<FileAccessor> FileSystemManager::CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
		auto accessor = CreateFileAccessorCore(id, manipulator, mode);
		return accessor;
	}

	std::shared_ptr<FileAccessor> FileSystemManager::GetFileAccessor(const std::string& id) const {
		auto it = accessors_.find(id);
		if (it == accessors_.end()) { return nullptr; } else {
			return it->second;
		}
	}

	std::shared_ptr<FileAccessor> FileSystemManager::CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
		assert(is_initialized_ == false); //これは初期化前に呼ぶ
		auto accessor = std::make_shared<FileAccessor>(manipulator, mode);
		accessors_.emplace(id, accessor);
		return std::move(accessor);
	}
}
