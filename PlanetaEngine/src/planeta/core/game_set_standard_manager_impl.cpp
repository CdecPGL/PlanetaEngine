#include "boost/filesystem.hpp"

#include "standard_resource_manager.hpp"
#include "standard_log_manager.hpp"
#include "standard_scene_manager.hpp"
#include "standard_input_manager.hpp"
#include "standard_performance_manager.hpp"
#include "standard_rendering_manager.hpp"
#include "standard_sound_manager.hpp"
#include "standard_save_manager.hpp"
#include "standard_debug_manager.hpp"
#include "standard_config_manager.hpp"
#include "system_variables.hpp"

namespace plnt {
	void game::set_standard_managers() const {
		using namespace private_;
		set_resource_manager(std::make_shared<standard_resource_manager>());
		const auto log_mgr = std::make_shared<standard_log_manager>();
		//開発モードならコンソールウインドウを作成し、そこにログを出力
		if (system_variables::development_mode) {
			//コンソール出力を有効化
			log_mgr->validate_console_out_put();
		}
		//ログディレクトリがなかったら作る
		if (!boost::filesystem::exists(system_variables::file_system::log_directory)) {
			boost::filesystem::create_directory(system_variables::file_system::log_directory);
		}
		//ログ出力ファイルを有効にする
		log_mgr->validate_file_out_put(
			system_variables::file_system::log_directory + "\\" + system_variables::file_system::log_out_put_file_name);
		set_log_manager(log_mgr);
		set_scene_manager(std::make_shared<standard_scene_manager>());
		set_input_manager(std::make_shared<standard_input_manager>());
		set_performance_manager(std::make_shared<standard_performance_manager>());
		set_rendering_manager(std::make_shared<standard_rendering_manager>());
		set_sound_manager(std::make_shared<standard_sound_manager>());
		set_save_manager(std::make_shared<standard_save_manager>());
		set_debug_manager(std::make_shared<standard_debug_manager>());
		set_config_manager(std::make_shared<standard_config_manager>());
	}
}
