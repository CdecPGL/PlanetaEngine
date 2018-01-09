#pragma once

#include"InputManager.hpp"

namespace plnt {
	namespace private_ {
		namespace key_input_io {
			int LoadKeyConfigFromINIFileToKIM(const std::string&, InputManager& kim); //キー設定をINIファイルから読み込み
			int SaveKeyConfigToINIFileFromKIM(const std::string&, int, const InputManager& kim); //キー設定をINIファイルへ保存(ファイル名,モード(0:文字列1:番号))
		}
	}
}