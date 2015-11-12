#pragma once

#include<string>
#include<vector>
#include <unordered_map>
#include"FileLoaderBase.h"
#include "PointerSingletonTemplate.h"

namespace planeta_engine{
	namespace file_system{
		class FileAccessor;
		class FileLoaderBase;
		class FileSaverBase;
		class FileLoadManager final: public utility::PointerSingletonTemplate<FileLoadManager>
		{
			friend utility::PointerSingletonTemplate<FileLoadManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileSaverBase>& saver);
			std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::vector<std::shared_ptr<FileLoaderBase>>& loaders);
			std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileSaverBase>& saver, const std::vector<std::shared_ptr<FileLoaderBase>>& loaders);
			std::shared_ptr<FileAccessor> GetFileAccessor(const std::string& id)const;
			bool DeleteCache();
			size_t GetCacheSize()const;
		private:
			FileLoadManager();
			FileLoadManager(const FileLoadManager&) = delete;
			FileLoadManager(FileLoadManager&&) = delete;
			FileLoadManager& operator=(const FileLoadManager&) = delete;
			FileLoadManager& operator=(FileLoadManager&&) = delete;
			~FileLoadManager();
			std::unordered_map<std::string, std::shared_ptr<FileAccessor>> accessors_;
			std::shared_ptr<FileAccessor> CreateFileAccessorCore(const std::string& id);
			bool is_initialized_ = false;
		};
	}
}