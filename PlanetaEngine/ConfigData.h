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
			//設定データを読み込む
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
				CONFIG_PARAMETER(Vector2Di, DrawSize);
				CONFIG_PARAMETER(Vector2Di, WindowSize);
				CONFIG_PARAMETER(unsigned int, ResourceDecryptionKey);
			};
			struct user {
				friend bool LoadConfigData(const std::string&);
				CONFIG_PARAMETER(bool, WindowSizeConfigurable); //ウインドウサイズをユーザーが設定可能か
				CONFIG_PARAMETER(bool, WindowModeConfigurable); //ウインドウモードか否かをユーザーが設定可能か
			};
		};
	}
}

#undef CONFIG_PARAMETER