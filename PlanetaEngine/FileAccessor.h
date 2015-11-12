#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Object.h"

/*������*/
namespace planeta_engine {
	namespace file_system {
		class File;
		class FileSaverBase;
		class FileLoaderBase;
		class FileAccessor final: public core::Object {
		public:
			FileAccessor() = default;
			~FileAccessor() = default;
			enum Mode { ReadOnly, WriteOnly, ReadWrite, Invalid };
			Mode mode()const { return mode_; }
			bool Initialize();
			void Finalize();
			/*���ʊ֐�*/
			void DeleteCache();
			size_t GetCacheSize()const;
			/*Read�֐�*/
			void PushLoader(const std::shared_ptr<FileLoaderBase>& loader);
			std::shared_ptr<const File> LoadFile(const std::string& file_name)const;
			bool LoadAllFilesToCache();
			/*Write�֐�*/
			void SetSaver(const std::shared_ptr<FileSaverBase>& saver);
			bool SaveFile(const std::string& file_name,const File& file);
			bool SaveFile(const std::string& file_name, File&& file);
			bool SaveAllFilesFromCache();
		private:
			std::vector<std::shared_ptr<FileLoaderBase>> loaders_;
			std::shared_ptr<FileSaverBase> saver_;
			mutable std::unordered_map<std::string, std::shared_ptr<File>> file_caches_;
			Mode mode_ = Mode::Invalid;
			std::shared_ptr<File> LoadProc(const std::string& file_name)const;
			bool SaveProc(const std::string& file_name,const File& file);
			bool is_initialized_ = false;
		};
	}
}