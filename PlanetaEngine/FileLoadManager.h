#pragma once

#include<string>
#include<vector>
#include <unordered_map>
#include"FileManipulatorBase.h"
#include "PointerSingletonTemplate.h"

namespace planeta_engine{
	namespace file_system{
		class FileAccessor;
		class FileManipulatorBase;
		enum class AccessMode;
		class FileLoadManager final: public utility::PointerSingletonTemplate<FileLoadManager>
		{
			friend utility::PointerSingletonTemplate<FileLoadManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
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
			std::shared_ptr<FileAccessor> CreateFileAccessorCore(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
			bool is_initialized_ = false;
		};
	}
}