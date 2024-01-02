#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "init_functions.hpp"
#include "system_variables.hpp"
#include "file_manipulator.hpp"
#include "file.hpp"
#include "json_file.hpp"
#include "config_manager.hpp"

namespace plnt::private_::init_funcs {
	bool load_config(config_manager &mgr, const std::shared_ptr<file_manipulator> &sys_dir_manipulator,
	                 const std::shared_ptr<file_manipulator> &cfg_dir_manipulator) {
		const auto sc_file = sys_dir_manipulator->load_file(system_variables::file_system::system_config_file_name);
		if (sc_file == nullptr) {
			PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::system_config_file_name, "\"が開けませんでした。");
			return false;
		}
		if (!mgr.load_system_config(*sc_file)) {
			PE_LOG_FATAL("システム設定ファイル\"", system_variables::file_system::system_config_file_name,
			             "\"の読み込みに失敗しました。");
			return false;
		}
		const auto uc_file = cfg_dir_manipulator->load_file(system_variables::file_system::user_config_file_name);
		if (uc_file == nullptr) {
			PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::user_config_file_name, "\"が開けませんでした。");
			return false;
		}
		if (!mgr.load_user_config(*uc_file)) {
			PE_LOG_FATAL("ユーザー設定ファイル\"", system_variables::file_system::user_config_file_name, "\"の読み込みに失敗しました。");
			return false;
		}
		return true;
	}
}
