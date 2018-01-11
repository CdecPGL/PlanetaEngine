#include <algorithm>

#include "planeta/core/FileIStream.hpp"
#include "planeta/core/FileSystemUtility.hpp"
#include "planeta/core/CharacterCode.hpp"

#include "RIni.hpp"

namespace plnt {

	const plnt::IniFile& RIni::ini_file() const {
		return *ini_file_;
	}

	plnt::IniFile& RIni::ini_file() {
		return *ini_file_;
	}

	bool RIni::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		ini_file_ = std::make_unique<IniFile>();
		if (!ini_file_->Load(file)) {
			PE_LOG_ERROR("INIファイルの読み込みに失敗しました。");
			ini_file_.release();
			return false;
		}
		return true;
	}

	void RIni::OnDisposed() {
		ini_file_.release();
	}

}