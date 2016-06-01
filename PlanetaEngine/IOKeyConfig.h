#pragma once

#include"KeyInputManager.h"

namespace planeta {
	namespace core {
		class KeyInputManager;
		namespace key_input_io {
			int LoadKeyConfigFromINIFileToKIM(const std::string&, KeyInputManager& kim); //�L�[�ݒ��INI�t�@�C������ǂݍ���
			int SaveKeyConfigToINIFileFromKIM(const std::string&, int, const KeyInputManager& kim); //�L�[�ݒ��INI�t�@�C���֕ۑ�(�t�@�C����,���[�h(0:������1:�ԍ�))
		}
	}
}