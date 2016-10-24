#include "StandardFileSystemManager.h"
#include "LogUtility.h"
#include "FileAccessor.h"
#include <cassert>

namespace planeta {
	namespace private_ {
		StandardFileSystemManager::StandardFileSystemManager() {

		}


		StandardFileSystemManager::~StandardFileSystemManager() {

		}

		bool StandardFileSystemManager::Initialize() {
			bool error = false;
			if (error) {
				PE_LOG_ERROR("初期化に失敗しました。FileAccessorの初期化に失敗しました。");
			}
			if (!error) { is_initialized_ = true; }
			return !error;
		}

		void StandardFileSystemManager::Finalize() {
			for (auto& accessor : accessors_) {
				accessor.second->Finalize();
			}
			is_initialized_ = false;
		}

		size_t StandardFileSystemManager::GetCacheSize()const {
			size_t res = 0;
			for (auto& accessor : accessors_) {
				res += accessor.second->GetCacheSize();
			}
			return res;
		}

		bool StandardFileSystemManager::DeleteCache() {
			for (auto& accessor : accessors_) {
				accessor.second->DeleteCache();
			}
			PE_LOG_MESSAGE("ファイルキャッシュを削除しました。");
			return true;
		}

		std::shared_ptr<FileAccessor> StandardFileSystemManager::CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
			auto accessor = CreateFileAccessorCore(id, manipulator, mode);
			if (accessor->Initialize()) {
				accessors_.emplace(id, accessor);
				return accessor;
			} else {
				PE_LOG_ERROR("ファイルアクセサの初期化に失敗しました。");
				return nullptr;
			}
		}

		std::shared_ptr<FileAccessor> StandardFileSystemManager::GetFileAccessor(const std::string& id) const {
			auto it = accessors_.find(id);
			if (it == accessors_.end()) { return nullptr; } else {
				return it->second;
			}
		}

		std::shared_ptr<FileAccessor> StandardFileSystemManager::CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) {
			assert(is_initialized_ == true); //これは初期化後に呼ぶ
			auto accessor = std::make_shared<FileAccessor>(manipulator, mode);
			return std::move(accessor);
		}

		bool StandardFileSystemManager::DisposeFileAccessor(const std::string& id) {
			auto it = accessors_.find(id);
			if (it == accessors_.end()) {
				PE_LOG_ERROR("指定されたファイルアクセサ\"", id, "\"は存在しないため破棄できませんでした。");
				return false;
			} else {
				it->second->Finalize();
				accessors_.erase(it);
				PE_LOG_MESSAGE("ファイルアクセサ\"", id, "\"を破棄しました。");
				return true;
			}
		}
	}
}
