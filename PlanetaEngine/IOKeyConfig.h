#pragma once

#include"KeyInputManager.h"

namespace planeta {
	namespace core {
		class KeyInputManager;
		namespace key_input_io {
			int LoadKeyConfigFromINIFileToKIM(const std::string&, KeyInputManager& kim); //キー設定をINIファイルから読み込み
			int SaveKeyConfigToINIFileFromKIM(const std::string&, int, const KeyInputManager& kim); //キー設定をINIファイルへ保存(ファイル名,モード(0:文字列1:番号))
		}
	}
}