#include <cassert>
#include "FileAccessor.h"
#include "FileLoaderBase.h"
#include "FileSaverBase.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {
		namespace {
			namespace access_mode {
				using type = uint_fast32_t;
				constexpr type None = 0b00;
				constexpr type Read = 0b01;
				constexpr type Write = 0b10;
				constexpr type ReadWrite = 0b11;
			}
			constexpr access_mode::type ConvertAccessModeToUint32(FileAccessor::Mode mode) {
				return mode == planeta_engine::file_system::FileAccessor::ReadOnly ? access_mode::Read :
					mode == planeta_engine::file_system::FileAccessor::WriteOnly ? access_mode::Write :
					mode == planeta_engine::file_system::FileAccessor::ReadWrite ? access_mode::ReadWrite :
					mode == planeta_engine::file_system::FileAccessor::Invalid ? access_mode::None : access_mode::None;
			}
			constexpr FileAccessor::Mode ConvertUint32ToAccessMode(access_mode::type amode_uint32) {
				return amode_uint32 == access_mode::Read ? FileAccessor::Mode::ReadOnly :
					amode_uint32 == access_mode::Write ? FileAccessor::Mode::WriteOnly :
					amode_uint32 == access_mode::ReadWrite ? FileAccessor::Mode::ReadWrite :
					amode_uint32 == access_mode::None ? FileAccessor::Mode::Invalid : FileAccessor::Mode::Invalid;
			}
		}

		void FileAccessor::PushLoader(const std::shared_ptr<FileLoaderBase>& loader)
		{
			if (is_initialized_) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ローダーの追加は初期化前に行う必要があります。");
				return;
			}
			mode_ = ConvertUint32ToAccessMode(ConvertAccessModeToUint32(mode_) | access_mode::Read);
			loaders_.push_back(loader);
		}

		std::shared_ptr<const File> FileAccessor::LoadFile(const std::string& file_name) const
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "読み込み属性がありません。");
				return nullptr;
			}
			auto it = file_caches_.find(file_name);
			if (it == file_caches_.end()) {
				auto file_ptr = LoadProc(file_name);
				if (file_ptr) {
					file_caches_.emplace(file_name, file_ptr);
					return file_ptr;
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイルを読み込めませんでした。");
					return nullptr;
				}
			}
			else {
				return it->second;
			}
		}

		bool FileAccessor::LoadAllFilesToCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "読み込み属性がありません。");
				return false;
			}
			for (auto& loader : loaders_) {
				auto files = std::move(loader->LoadAllFiles());
				for (auto file : files) {
					file_caches_.emplace(file.first, file.second);
				}
			}
			return true;
		}

		void FileAccessor::SetSaver(const std::shared_ptr<FileSaverBase>& saver)
		{
			if (is_initialized_) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "Saverの設定は初期化前に行う必要があります。");
				return;
			}
			mode_ = ConvertUint32ToAccessMode(ConvertAccessModeToUint32(mode_) | access_mode::Write);
			saver_ = saver;
		}

		bool FileAccessor::SaveFile(const std::string& file_name, const File& file)
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
				return false;
			}
			*file_caches_[file_name] = file;
			return true;
		}

		bool FileAccessor::SaveFile(const std::string& file_name, File&& file)
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
				return false;
			}
			*file_caches_[file_name] = std::move(file);
			return true;
		}

		bool FileAccessor::SaveAllFilesFromCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
				return false;
			}

			std::vector<std::pair<std::string,const File&>> files;
			for (const auto& fc : file_caches_) {
				files.push_back(std::make_pair(fc.first, *fc.second));
			}
			return saver_->SaveFiles(files);
		}

		std::shared_ptr<File> FileAccessor::LoadProc(const std::string& file_name) const
		{
			assert(loaders_.size() > 0);
			std::shared_ptr<File> file;
			for (auto& ldr : loaders_) {
				if ((file = ldr->LoadFile(file_name)) != nullptr) { break; }
			}
			if (!file) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", file_name, "が読み込めませんでした。");
			}
			return file;
		}

		bool FileAccessor::SaveProc(const std::string& file_name, const File& file)
		{
			assert(saver_ != nullptr);
			return saver_->SaveFile(file_name, file);
		}

		void FileAccessor::DeleteCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) == access_mode::Write) {
				SaveAllFilesFromCache();
			}
			file_caches_.clear();
		}

		size_t FileAccessor::GetCacheSize() const
		{
			size_t size = 0;
			for (auto& c : file_caches_) { size += c.second->GetSize(); }
			return size;
		}

		bool FileAccessor::Initialize()
		{
			if (is_initialized_) { return true; } //初期化済みだったら飛ばす
			bool saver_error = false;
			if (saver_) { 
				if (!saver_->Initialize()) {
					debug::SystemLog::instance().LogMessage("FileSaverの初期化に失敗しました。", __FUNCTION__);
					saver_error = true;
				}
			}
			bool error_flag = false;
			bool success_flag = false;
			for (auto& loader : loaders_) {
				if (loader->Initialize()) {
					success_flag = true;
				}
				else {
					error_flag = true;
				}
			}
			is_initialized_ = true;
			if ((!success_flag && error_flag) || saver_error ) {
				debug::SystemLog::instance().LogError("初期化に失敗しました。", __FUNCTION__);
				return true;
			}
			else if (success_flag && error_flag) {
				debug::SystemLog::instance().LogWarning("一部ローダーの初期化に失敗しましたが、継続します。", __FUNCTION__);
				return true;
			}
			else if(!saver_error && success_flag && !error_flag){
				debug::SystemLog::instance().LogMessage("初期化に成功しました。", __FUNCTION__);
				return false;
			}
			else {
				assert(false);
				return false;
			}
		}

		void FileAccessor::Finalize()
		{
			if (!is_initialized_) { return; } //初期化されていなかったら飛ばす
			if (saver_) { saver_->Finalize(); }
			for (auto& loader : loaders_) {
				loader->Finalize();
			}
			is_initialized_ = false;
		}

	}
}