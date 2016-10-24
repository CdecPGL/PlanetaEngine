#pragma once

#include<string>
#include<vector>
#include <unordered_map>
#include "FileSystemManager.h"
#include"FileManipulatorBase.h"

namespace planeta {
	namespace private_ {
		class StandardFileSystemManager final : public FileSystemManager {
		public:
			StandardFileSystemManager();
			~StandardFileSystemManager();
			StandardFileSystemManager(const StandardFileSystemManager&) = delete;
			StandardFileSystemManager(StandardFileSystemManager&&) = delete;
			bool Initialize()override;
			void Finalize()override;
			std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode)override;
			std::shared_ptr<FileAccessor> GetFileAccessor(const std::string& id)const override;
			bool DisposeFileAccessor(const std::string& id)override;
			bool DeleteCache()override;
			size_t GetCacheSize()const override;
		private:
			StandardFileSystemManager& operator=(const StandardFileSystemManager&) = delete;
			StandardFileSystemManager& operator=(StandardFileSystemManager&&) = delete;
			std::unordered_map<std::string, std::shared_ptr<FileAccessor>> accessors_;
			std::shared_ptr<FileAccessor> CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
			bool is_initialized_ = false;
		};
	}
}