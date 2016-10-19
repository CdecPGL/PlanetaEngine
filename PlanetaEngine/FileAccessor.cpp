#include <cassert>
#include "FileAccessor.h"
#include "FileManipulatorBase.h"
#include "SystemLog.h"
#include "FileAccessMode.h"

namespace planeta {
	namespace {
		namespace access_mode {
			using type = uint_fast32_t;
			constexpr type None = 0b00;
			constexpr type Read = 0b01;
			constexpr type Write = 0b10;
			constexpr type ReadWrite = 0b11;
		}
		constexpr access_mode::type ConvertAccessModeToUint32(AccessMode mode) {
			return mode == planeta::AccessMode::ReadOnly ? access_mode::Read :
				mode == planeta::AccessMode::WriteOnly ? access_mode::Write :
				mode == planeta::AccessMode::ReadWrite ? access_mode::ReadWrite :
				mode == planeta::AccessMode::Invalid ? access_mode::None : access_mode::None;
		}
		constexpr AccessMode ConvertUint32ToAccessMode(access_mode::type amode_uint32) {
			return amode_uint32 == access_mode::Read ? AccessMode::ReadOnly :
				amode_uint32 == access_mode::Write ? AccessMode::WriteOnly :
				amode_uint32 == access_mode::ReadWrite ? AccessMode::ReadWrite :
				amode_uint32 == access_mode::None ? AccessMode::Invalid : AccessMode::Invalid;
		}
	}

	std::shared_ptr<const File> FileAccessor::LoadFile(const std::string& file_name) const {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return nullptr;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Read) != access_mode::Read) {
			PE_LOG_ERROR("読み込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
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
				PE_LOG_ERROR("ファイルを読み込めませんでした。");
				return nullptr;
			}
		} else {
			return it->second;
		}
	}

	bool FileAccessor::LoadAllFilesToCache() {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Read) != access_mode::Read) {
			PE_LOG_ERROR("読み込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return false;
		}
		assert(manipulator_ != nullptr);
		auto path_list = manipulator_->GetAllFilePaths();
		bool scc = true;
		for (auto&& p : path_list) {
			auto file = manipulator_->LoadFile(p);
			if (file == nullptr) { scc = false; continue; }
			file_caches_.emplace(file->file_name(), file);
		}
		return scc;
	}

	bool FileAccessor::SaveFile(const std::string& file_name, const File& file) {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Write) != access_mode::Write) {
			PE_LOG_ERROR("書き込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return false;
		}
		assert(manipulator_ != nullptr);
		*file_caches_[file_name] = file;
		return true;
	}

	bool FileAccessor::SaveFile(const std::string& file_name, File&& file) {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Write) != access_mode::Write) {
			PE_LOG_ERROR("書き込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return false;
		}
		assert(manipulator_ != nullptr);
		*file_caches_[file_name] = std::move(file);
		return true;
	}

	bool FileAccessor::SaveAllFilesFromCache() {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Write) != access_mode::Write) {
			PE_LOG_ERROR("書き込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return false;
		}
		assert(manipulator_ != nullptr);
		bool scc = true;
		for (const auto& fc : file_caches_) {
			scc &= manipulator_->SaveFile(fc.first, *fc.second);
		}
		return scc;
	}

	std::shared_ptr<File> FileAccessor::LoadProc(const std::string& file_name) const {
		assert(manipulator_ != nullptr);
		auto file = manipulator_->LoadFile(file_name);
		if (!file) {
			PE_LOG_ERROR("ファイル", file_name, "が読み込めませんでした。");
		}
		return file;
	}

	bool FileAccessor::SaveProc(const std::string& file_name, const File& file) {
		assert(manipulator_ != nullptr);
		return manipulator_->SaveFile(file_name, file);
	}

	void FileAccessor::DeleteCache() {
		if (is_valid() == false) {
			PE_LOG_ERROR("無効なファイルアクセサです。初期化されていないか、破棄された可能性があります。");
			return;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Write) == access_mode::Write) {
			SaveAllFilesFromCache();
		}
		file_caches_.clear();
	}

	size_t FileAccessor::GetCacheSize() const {
		size_t size = 0;
		for (auto& c : file_caches_) { size += c.second->size(); }
		return size;
	}

	bool FileAccessor::Initialize() {
		if (is_initialized_) {  //初期化済みだったら飛ばす
			PE_LOG_WARNING("すでに初期化されています。");
			return true;
		}
		if (manipulator_) {
			if (!manipulator_->Initialize()) {
				PE_LOG_ERROR("初期化に失敗しました。ファイルマニピュレータの初期化に失敗しました。");
				return false;
			} else {
//				PE_LOG_MESSAGE("初期化に成功しました。");
				is_initialized_ = true;
				return true;
			}
		} else {
			PE_LOG_ERROR("初期化に失敗しました。ファイルマニピュレータがセットされていません。");
			return false;
		}
	}

	void FileAccessor::Finalize() {
		if (!is_initialized_) { //初期化されていなかったら飛ばす
			PE_LOG_WARNING("終了処理を行う必要はありません。");
			return; 
		} 
		if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) == access_mode::Write) { //書き込み属性があったら
			SaveAllFilesFromCache(); //キャッシュの内容をすべて保存する。
		}
		if (manipulator_) { manipulator_->Finalize(); }
		is_initialized_ = false;
	}

	FileAccessor::FileAccessor(const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) :manipulator_(manipulator), mode_(mode) {

	}

	bool FileAccessor::is_valid() const {
		return is_initialized_;
	}

}