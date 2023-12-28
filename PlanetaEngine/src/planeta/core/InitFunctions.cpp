#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.hpp"
#include "SystemVariables.hpp"
#include "file_manipulator.hpp"
#include "file.hpp"
#include "JsonFile.hpp"
#include "config_manager.hpp"

namespace plnt {
	namespace private_ {
		namespace init_funcs {
			bool LoadConfig(config_manager &mgr, const std::shared_ptr<file_manipulator> &sys_dir_manipurator,
			                const std::shared_ptr<file_manipulator> &cfg_dir_manipurator) {
				auto sc_file = sys_dir_manipurator->load_file(system_variables::file_system::SystemConfigFileName);
				if (sc_file == nullptr) {
					PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::SystemConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (!mgr.load_system_config(*sc_file)) {
					PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::SystemConfigFileName,
					             "\"の読み込みに失敗しました。");
					return false;
				}
				auto uc_file = cfg_dir_manipurator->load_file(system_variables::file_system::UserConfigFileName);
				if (uc_file == nullptr) {
					PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::UserConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (!mgr.load_user_config(*uc_file)) {
					PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::UserConfigFileName, "\"の読み込みに失敗しました。");
					return false;
				}
				return true;
			}
		}
	}
}
