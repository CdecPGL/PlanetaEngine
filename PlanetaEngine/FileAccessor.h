#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "FileAccessMode.h"

namespace planeta {
	class File;
	class FileManipulatorBase;
	class FileAccessor final : public Object {
	public:
		FileAccessor(const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode);
		~FileAccessor() = default;
		AccessMode mode()const { return mode_; }
		bool Initialize();
		void Finalize();
		/*共通関数*/
		void DeleteCache();
		size_t GetCacheSize()const;
		/*Read関数*/
		std::shared_ptr<const File> LoadFile(const std::string& file_name)const;
		bool LoadAllFilesToCache();
		/*Write関数*/
		bool SaveFile(const std::string& file_name, const File& file);
		bool SaveFile(const std::string& file_name, File&& file);
		bool SaveAllFilesFromCache();

		bool is_valid()const;
	private:
		std::shared_ptr<FileManipulatorBase> manipulator_;
		mutable std::unordered_map<std::string, std::shared_ptr<File>> file_caches_;
		AccessMode mode_ = AccessMode::Invalid;
		std::shared_ptr<File> LoadProc(const std::string& file_name)const;
		bool SaveProc(const std::string& file_name, const File& file);
		bool is_initialized_ = false;
	};
}