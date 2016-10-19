#include "FileManipulatorBase.h"
#include "SystemLog.h"
#include "EncrypterBase.h"

namespace planeta {
	bool FileManipulatorBase::CheckFileExist(const std::string& file_name) const {
		return CheckFileExistenceProc(file_name);
	}

	std::vector<std::string> FileManipulatorBase::GetAllFilePaths() const {
		std::vector<std::string> out{};
		if (GetAllFilePathsProc(out)) {
			return std::move(out);
		} else {
			PE_LOG_ERROR("ファイルパスリストの取得に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return{};
		}
	}

	boost::optional<const encrypters::EncrypterBase&> FileManipulatorBase::encrypter() const& {
		if (encrypter_) {
			return *encrypter_;
		} else {
			return boost::none;
		}
	}

	std::shared_ptr<File> FileManipulatorBase::LoadFile(const std::string& name) {
		auto file = std::make_shared<File>();
		if (LoadFileProc(name, *file)) {
			file->SetFileName(name);
			return std::move(file);
		} else {
			PE_LOG_ERROR("ファイル", name, "の読み込みに失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return nullptr;
		}
	}

	FileManipulatorBase::FileManipulatorBase(const std::string& p, bool auto_create) :root_path_(p), is_valid_(false), auto_create_(auto_create) {

	}

	FileManipulatorBase::FileManipulatorBase(const std::string& p, std::unique_ptr<const encrypters::EncrypterBase>&& encrypter, bool auto_create) : root_path_(p), encrypter_(std::move(encrypter)), is_valid_(false), auto_create_(auto_create) {

	}

	FileManipulatorBase::~FileManipulatorBase() = default;

	bool FileManipulatorBase::Initialize() {
		if (is_valid_) { return true; }
		if (InitializeProc()) {
			is_valid_ = true;
			PE_LOG_MESSAGE("初期化されました。(パス ", root_path(), ",ファイル数 ", GetFileCountProc(), ",タイプ ", typeid(*this).name(), ")");
			return true;
		} else {
			PE_LOG_ERROR("初期化に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	void FileManipulatorBase::Finalize() {
		if (!is_valid_) { return; }
		FinalizeProc();
		is_valid_ = false;
	}

	bool FileManipulatorBase::SaveFile(const std::string& name, const File& file) {
		if (SaveFileProc(name, file)) {
			return true;
		} else {
			PE_LOG_ERROR("ファイル", name, "の保存に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	bool FileManipulatorBase::Reload() {
		if (!ReloadProc()) {
			PE_LOG_ERROR("管理情報の更新に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		} else {
			return true;
		}
	}

}