#include "FileLoaderBase.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {

		bool FileLoaderBase::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files)
		{
			bool err = false;
			for (const auto& fn : file_list_) {
				auto file = std::make_shared<File>();
				if (!LoadFileCore(fn, *file)) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", fn, "の読み込みに失敗しました。(パス ", path(), ",タイプ ", GetType().name(), ")");
					err = true;
				}
				else {
					files.push_back(std::make_pair(fn, file));
				}
			}
			return !err;
		}

		bool FileLoaderBase::CheckFileExist(const std::string& file_name) const
		{
			return file_list_.find(file_name) != file_list_.end();
		}

		std::shared_ptr<File> FileLoaderBase::LoadFile(const std::string& name)
		{
			if (file_list_.find(name) == file_list_.end()) { return nullptr; }
			auto file = std::make_shared<File>();
			if (LoadFileCore(name, *file)) {
				return std::move(file);
			}
			else {
				return nullptr;
			}
		}

		std::vector<std::pair<std::string, std::shared_ptr<File>>> FileLoaderBase::LoadAllFiles()
		{
			UpdateFileList();
			std::vector<std::pair<std::string, std::shared_ptr<File>>> ret;
			if (LoadAllFilesCore(ret)) {
				return std::move(ret);
			}
			else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイルの全ロードに失敗しました。(パス ", path(), ",タイプ ", GetType().name(), ")");
				return decltype(LoadAllFiles())();
			}
		}

		bool FileLoaderBase::Initialize()
		{
			if (is_valid_) { return true; }
			if (InitializeCore()) { 
				is_valid_ = true; 
				if (UpdateFileList()) {
					debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "初期化されました。(パス ", path(), ",ファイル数 ", file_list_.size(), ",タイプ ", GetType().name(), ")");
					return true;
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。(パス ", path(), ",タイプ ", GetType().name(), ")");
					return false;
				}
			}
			else { 
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。(パス ", path(), ",タイプ ", GetType().name(), ")");
				return false;
			}
		}

	}
}