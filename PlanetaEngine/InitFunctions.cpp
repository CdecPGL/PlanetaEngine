﻿#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "FileManipulatorBase.h"
#include "File.h"
#include "JsonFile.h"
#include "ConfigManager.h"

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			bool LoadConfig(ConfigManager& mgr, const std::shared_ptr<FileManipulatorBase>& sys_dir_manipurator, const std::shared_ptr<FileManipulatorBase>& cfg_dir_manipurator) {
				auto sc_file = sys_dir_manipurator->LoadFile(system_variables::file_system::SystemConfigFileName);
				if (sc_file == nullptr) {
					PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::SystemConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (!mgr.LoadSystemConfig(*sc_file)) {
					PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::SystemConfigFileName, "\"の読み込みに失敗しました。");
					return false;
				}
				auto uc_file = sys_dir_manipurator->LoadFile(system_variables::file_system::UserConfigFileName);
				if (uc_file == nullptr) {
					PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::UserConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (!mgr.LoadUserConfig(*uc_file)) {
					PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::UserConfigFileName, "\"の読み込みに失敗しました。");
					return false;
				}
				return true;
			}
		}
	}
}
