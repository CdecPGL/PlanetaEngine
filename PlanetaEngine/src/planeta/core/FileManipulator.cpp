#include "FileManipulator.hpp"
#include "LogUtility.hpp"
#include "planeta/archiver/EncrypterBase.hpp"

namespace plnt {
	namespace {
		namespace access_mode {
			using type = uint_fast32_t;
			constexpr type None = 0b00;
			constexpr type Read = 0b01;
			constexpr type Write = 0b10;
			constexpr type ReadWrite = 0b11;
		}

		constexpr access_mode::type ConvertAccessModeToUint32(AccessMode mode) {
			return mode == plnt::AccessMode::ReadOnly
				       ? access_mode::Read
				       : mode == plnt::AccessMode::WriteOnly
				       ? access_mode::Write
				       : mode == plnt::AccessMode::ReadWrite
				       ? access_mode::ReadWrite
				       : mode == plnt::AccessMode::Invalid
				       ? access_mode::None
				       : access_mode::None;
		}

		constexpr AccessMode ConvertUint32ToAccessMode(access_mode::type amode_uint32) {
			return amode_uint32 == access_mode::Read
				       ? AccessMode::ReadOnly
				       : amode_uint32 == access_mode::Write
				       ? AccessMode::WriteOnly
				       : amode_uint32 == access_mode::ReadWrite
				       ? AccessMode::ReadWrite
				       : amode_uint32 == access_mode::None
				       ? AccessMode::Invalid
				       : AccessMode::Invalid;
		}
	}

	bool FileManipulator::CheckFileExist(const std::string &file_name) const {
		return CheckFileExistenceProc(file_name);
	}

	std::vector<std::string> FileManipulator::GetAllFilePaths() const {
		std::vector<std::string> out{};
		if (GetAllFilePathsProc(out)) { return std::move(out); } else {
			PE_LOG_ERROR("ファイルパスリストの取得に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return {};
		}
	}

	AccessMode FileManipulator::mode() const { return mode_; }

	boost::optional<const archiver::EncrypterBase &> FileManipulator::encrypter() const & {
		if (encrypter_) { return *encrypter_; } else { return boost::none; }
	}

	std::shared_ptr<File> FileManipulator::LoadFile(const std::string &name) {
		if (is_opened() == false) {
			PE_LOG_ERROR("無効なファイルマニピュレータです。初期化されていないか、破棄された可能性があります。");
			return nullptr;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Read) != access_mode::Read) {
			PE_LOG_ERROR("読み込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return nullptr;
		}
		auto file = std::make_shared<File>();
		if (LoadFileProc(name, *file)) {
			file->SetFileName(name);
			return std::move(file);
		} else {
			PE_LOG_ERROR("ファイル", name, "の読み込みに失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return nullptr;
		}
	}

	FileManipulator::FileManipulator() : root_path_(""), mode_(AccessMode::Invalid), is_opened_(false),
	                                     auto_create_(false) { }

	FileManipulator::~FileManipulator() = default;

	bool FileManipulator::Open(const std::string &path, AccessMode access_mode, bool auto_create) {
		if (is_opened_) {
			PE_LOG_ERROR("すでに開かれています。");
			return false;
		}
		mode_ = access_mode;
		auto_create_ = auto_create;
		root_path_ = path;
		if (OpenProc(path)) {
			is_opened_ = true;
			PE_LOG_MESSAGE("初期化されました。(パス ", root_path(), ",ファイル数 ", GetFileCountProc(), ",タイプ ", typeid(*this).name(),
			               ")");
			return true;
		} else {
			PE_LOG_ERROR("初期化に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			mode_ = AccessMode::Invalid;
			auto_create_ = false;
			root_path_ = "";
			encrypter_ = nullptr;
			return false;
		}
	}

	bool FileManipulator::Open(const std::string &path, AccessMode access_mode,
	                           std::unique_ptr<const archiver::EncrypterBase> &&encrypter, bool auto_create) {
		encrypter_ = std::move(encrypter);
		return Open(path, access_mode, auto_create);
	}

	void FileManipulator::Close() {
		if (!is_opened_) {
			PE_LOG_WARNING("開かれる前に閉じられようとしました。");
			return;
		}
		CloseProc();
		mode_ = AccessMode::Invalid;
		auto_create_ = false;
		root_path_ = "";
		encrypter_ = nullptr;
		is_opened_ = false;
	}

	bool FileManipulator::SaveFile(const std::string &name, const File &file) {
		if (is_opened() == false) {
			PE_LOG_ERROR("無効なファイルマニピュレータです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((ConvertAccessModeToUint32(mode_) & access_mode::Write) != access_mode::Write) {
			PE_LOG_ERROR("書き込み属性がありません。value:", ConvertAccessModeToUint32(mode_));
			return false;
		}
		if (SaveFileProc(name, file)) { return true; } else {
			PE_LOG_ERROR("ファイル", name, "の保存に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	bool FileManipulator::Reload() {
		if (!ReloadProc()) {
			PE_LOG_ERROR("管理情報の更新に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		} else { return true; }
	}
}
