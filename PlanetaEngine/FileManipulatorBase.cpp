#include "FileManipulatorBase.h"
#include "SystemLog.h"
#include "EncrypterBase.h"

namespace planeta {
	bool FileManipulatorBase::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files) {
		bool err = false;
		for (const auto& fn : file_list_) {
			auto file = std::make_shared<File>();
			if (!LoadFileCore(fn, *file)) {
				PE_LOG_ERROR("ファイル", fn, "の読み込みに失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
				err = true;
			} else {
				files.push_back(std::make_pair(fn, file));
			}
		}
		return !err;
	}

	bool FileManipulatorBase::CheckFileExist(const std::string& file_name) const {
		return file_list_.find(file_name) != file_list_.end();
	}

	boost::optional<const encrypters::EncrypterBase&> FileManipulatorBase::encrypter() const& {
		if (encrypter_) {
			return *encrypter_;
		} else {
			return boost::none;
		}
	}

	std::shared_ptr<File> FileManipulatorBase::LoadFile(const std::string& name) {
		if (file_list_.find(name) == file_list_.end()) { return nullptr; }
		auto file = std::make_shared<File>();
		if (LoadFileCore(name, *file)) {
			file->SetFileName(name);
			return std::move(file);
		} else {
			return nullptr;
		}
	}

	std::vector<std::shared_ptr<File>> FileManipulatorBase::LoadAllFiles() {
		UpdateFileList();
		std::vector<std::pair<std::string, std::shared_ptr<File>>> ret;
		if (LoadAllFilesCore(ret)) {
			std::vector<std::shared_ptr<File>> out;
			for (auto&& pf : ret) {
				pf.second->SetFileName(pf.first);
				out.push_back(pf.second);
			}
			return std::move(out);
		} else {
			PE_LOG_ERROR("ファイルの全ロードに失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
			return{};
		}
	}

	FileManipulatorBase::FileManipulatorBase(const std::string& p, bool auto_create) :path_(p), is_valid_(false), auto_create_(auto_create) {

	}

	FileManipulatorBase::FileManipulatorBase(const std::string& p, std::unique_ptr<const encrypters::EncrypterBase>&& encrypter, bool auto_create) : path_(p), encrypter_(std::move(encrypter)), is_valid_(false), auto_create_(auto_create) {

	}

	FileManipulatorBase::~FileManipulatorBase() = default;

	bool FileManipulatorBase::Initialize() {
		if (is_valid_) { return true; }
		if (InitializeCore()) {
			is_valid_ = true;
			if (UpdateFileList()) {
				PE_LOG_MESSAGE("初期化されました。(パス ", path(), ",ファイル数 ", file_list_.size(), ",タイプ ", typeid(*this).name(), ")");
				return true;
			} else {
				PE_LOG_ERROR("初期化に失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
				return false;
			}
		} else {
			PE_LOG_ERROR("初期化に失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	void FileManipulatorBase::Finalize() {
		if (!is_valid_) { return; }
		FinalizeCore();
		is_valid_ = false;
	}

	bool FileManipulatorBase::SaveFile(const std::string& name, const File& file) {
		if (SaveFileCore(name, file)) {
			file_list_.emplace(name);
			return true;
		} else {
			PE_LOG_ERROR("ファイル,name,の保存に失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	bool FileManipulatorBase::SaveFiles(const std::vector<std::pair<std::string, const File&>>& files) {
		bool ret = SaveFilesCore(files);
		if (!ret) {
			PE_LOG_ERROR("複数ファイルの保存に失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
		}
		UpdateFileList();
		return ret;
	}

	bool FileManipulatorBase::SaveFilesCore(const std::vector<std::pair<std::string, const File&>>& files) {
		bool err = false;
		for (const auto& f : files) {
			if (!SaveFile(f.first, f.second)) {
				PE_LOG_ERROR("ファイル", f.first, "の保存に失敗しました。(パス ", path(), ",タイプ ", typeid(*this).name(), ")");
				err = true;
			}
		}
		return !err;
	}
}