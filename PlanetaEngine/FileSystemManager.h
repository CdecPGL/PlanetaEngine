#pragma once

#include<string>
#include<vector>
#include <unordered_map>
#include"FileManipulatorBase.h"
#include "SingletonTemplate.h"

namespace planeta {
	class FileAccessor;
	class FileManipulatorBase;
	enum class AccessMode;
	class FileSystemManager final : public util::SingletonTemplate<FileSystemManager> {
		friend util::SingletonTemplate<FileSystemManager>;
	public:
		bool Initialize()override;
		void Finalize()override;
		std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
		std::shared_ptr<FileAccessor> GetFileAccessor(const std::string& id)const;
		bool DisposeFileAccessor(const std::string& id);
		bool DeleteCache();
		size_t GetCacheSize()const;
	private:
		FileSystemManager();
		FileSystemManager(const FileSystemManager&) = delete;
		FileSystemManager(FileSystemManager&&) = delete;
		FileSystemManager& operator=(const FileSystemManager&) = delete;
		FileSystemManager& operator=(FileSystemManager&&) = delete;
		~FileSystemManager();
		std::unordered_map<std::string, std::shared_ptr<FileAccessor>> accessors_;
		std::shared_ptr<FileAccessor> CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
		bool is_initialized_ = false;
	};
}