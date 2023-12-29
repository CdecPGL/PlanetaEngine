#pragma once

#include"input_manager.hpp"

namespace plnt::private_::key_input_io {
	// キー設定をINIファイルから読み込み
	int load_key_config_from_ini_file_to_kim(const std::string &, input_manager &kim);
	// キー設定をINIファイルへ保存(ファイル名,モード(0:文字列1:番号))
	int save_key_config_to_ini_file_from_kim(const std::string &, int, const input_manager &kim);
}
