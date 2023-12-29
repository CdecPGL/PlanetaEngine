#include <algorithm>

#include "..\core\file_istream.hpp"
#include "..\core\file_system_utility.hpp"
#include "..\core\character_code.hpp"

#include "RIni.hpp"

namespace plnt {
	const plnt::ini_file &RIni::get_ini_file() const { return *ini_file_; }

	plnt::ini_file &RIni::get_ini_file() { return *ini_file_; }

	bool RIni::OnLoaded(const file &file, const JsonFile &metadata, ResourceReferencer &referencer) {
		ini_file_ = std::make_unique<ini_file>();
		if (!ini_file_->load(file)) {
			PE_LOG_ERROR("INIファイルの読み込みに失敗しました。");
			ini_file_.release();
			return false;
		}
		return true;
	}

	void RIni::OnDisposed() { ini_file_.release(); }
}
