#include <cassert>
#include "FileAccessor.h"
#include "FileManipulatorBase.h"
#include "SystemLog.h"
#include "FileAccessMode.h"

namespace planeta_engine {
	namespace {
		namespace access_mode {
			using type = uint_fast32_t;
			constexpr type None = 0b00;
			constexpr type Read = 0b01;
			constexpr type Write = 0b10;
			constexpr type ReadWrite = 0b11;
		}
		constexpr access_mode::type ConvertAccessModeToUint32(AccessMode mode) {
			return mode == planeta_engine::AccessMode::ReadOnly ? access_mode::Read :
				mode == planeta_engine::AccessMode::WriteOnly ? access_mode::Write :
				mode == planeta_engine::AccessMode::ReadWrite ? access_mode::ReadWrite :
				mode == planeta_engine::AccessMode::Invalid ? access_mode::None : access_mode::None;
		}
		constexpr AccessMode ConvertUint32ToAccessMode(access_mode::type amode_uint32) {
			return amode_uint32 == access_mode::Read ? AccessMode::ReadOnly :
				amode_uint32 == access_mode::Write ? AccessMode::WriteOnly :
				amode_uint32 == access_mode::ReadWrite ? AccessMode::ReadWrite :
				amode_uint32 == access_mode::None ? AccessMode::Invalid : AccessMode::Invalid;
		}
	}

	std::shared_ptr<const File> FileAccessor::LoadFile(const std::string& file_name) const {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "読み込み属性がありません。");
			return nullptr;
		}
		assert(manipulator_ != nullptr);
		auto it = file_caches_.find(file_name);
		if (it == file_caches_.end()) {
			auto file_ptr = LoadProc(file_name);
			if (file_ptr) {
				file_caches_.emplace(file_name, file_ptr);
				return file_ptr;
			} else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイルを読み込めませんでした。");
				return nullptr;
			}
		} else {
			return it->second;
		}
	}

	bool FileAccessor::LoadAllFilesToCache() {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "読み込み属性がありません。");
			return false;
		}
		assert(manipulator_ != nullptr);
		auto files = std::move(manipulator_->LoadAllFiles());
		for (auto file : files) {
			file_caches_.emplace(file.first, file.second);
		}
		return true;
	}

	bool FileAccessor::SaveFile(const std::string& file_name, const File& file) {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
			return false;
		}
		assert(manipulator_ != nullptr);
		*file_caches_[file_name] = file;
		return true;
	}

	bool FileAccessor::SaveFile(const std::string& file_name, File&& file) {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
			return false;
		}
		assert(manipulator_ != nullptr);
		*file_caches_[file_name] = std::move(file);
		return true;
	}

	bool FileAccessor::SaveAllFilesFromCache() {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "書き込み属性がありません。");
			return false;
		}
		assert(manipulator_ != nullptr);
		std::vector<std::pair<std::string, const File&>> files;
		for (const auto& fc : file_caches_) {
			files.push_back(std::make_pair(fc.first, *fc.second));
		}
		return manipulator_->SaveFiles(files);
	}

	std::shared_ptr<File> FileAccessor::LoadProc(const std::string& file_name) const {
		assert(manipulator_ != nullptr);
		auto file = manipulator_->LoadFile(file_name);
		if (!file) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", file_name, "が読み込めませんでした。");
		}
		return file;
	}

	bool FileAccessor::SaveProc(const std::string& file_name, const File& file) {
		assert(manipulator_ != nullptr);
		return manipulator_->SaveFile(file_name, file);
	}

	void FileAccessor::DeleteCache() {
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) == access_mode::Write) {
			SaveAllFilesFromCache();
		}
		file_caches_.clear();
	}

	size_t FileAccessor::GetCacheSize() const {
		size_t size = 0;
		for (auto& c : file_caches_) { size += c.second->GetSize(); }
		return size;
	}

	bool FileAccessor::Initialize() {
		if (is_initialized_) { return true; } //初期化済みだったら飛ばす
		if (manipulator_) {
			if (!manipulator_->Initialize()) {
				debug::SystemLog::instance().LogError("初期化に失敗しました。ファイルマニピュレータの初期化に失敗しました。", __FUNCTION__);
				return false;
			} else {
				debug::SystemLog::instance().LogMessage("初期化に成功しました。", __FUNCTION__);
				return true;
			}
		} else {
			debug::SystemLog::instance().LogError("初期化に失敗しました。ファイルマニピュレータがセットされていません。", __FUNCTION__);
			return false;
		}
	}

	void FileAccessor::Finalize() {
		if (!is_initialized_) { return; } //初期化されていなかったら飛ばす
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) == access_mode::Write) { //書き込み属性があったら
			SaveAllFilesFromCache(); //キャッシュの内容をすべて保存する。
		}
		if (manipulator_) { manipulator_->Finalize(); }
		is_initialized_ = false;
	}

	FileAccessor::FileAccessor(const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) :manipulator_(manipulator), mode_(mode) {

	}
}