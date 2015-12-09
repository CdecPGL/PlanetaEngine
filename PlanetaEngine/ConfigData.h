#pragma once

#include <string>
#include "Vector2D.h"

#define CONFIG_PARAMETER(type,param_name)\
private:\
static type param_name##_;\
public:\
static const type& param_name(){return param_name##_;}

namespace planeta_engine {
	namespace core {
		namespace config_data {
			//�ݒ�f�[�^��ǂݍ���
			bool LoadConfigData(const std::string& file_path);
			struct game {
				friend bool LoadConfigData(const std::string&);
				CONFIG_PARAMETER(std::string, GameTitle);
				CONFIG_PARAMETER(std::string, VersionString);
				CONFIG_PARAMETER(int, MajorVersionNumber);
				CONFIG_PARAMETER(int, MinorVersionNumber);
				CONFIG_PARAMETER(int, SubVersionNumber);
			};
			struct engine {
				friend bool LoadConfigData(const std::string&);
				CONFIG_PARAMETER(bool, WindowMode);
				CONFIG_PARAMETER(int, ColorBitDepth);
				CONFIG_PARAMETER(Vector2D<int>, DrawSize);
				CONFIG_PARAMETER(Vector2D<int>, WindowSize);
				CONFIG_PARAMETER(unsigned int, ResourceDecryptionKey);
			};
			struct user {
				friend bool LoadConfigData(const std::string&);
				CONFIG_PARAMETER(bool, WindowSizeConfigurable); //�E�C���h�E�T�C�Y�����[�U�[���ݒ�\��
				CONFIG_PARAMETER(bool, WindowModeConfigurable); //�E�C���h�E���[�h���ۂ������[�U�[���ݒ�\��
			};
		};
	}
}

#undef CONFIG_PARAMETER