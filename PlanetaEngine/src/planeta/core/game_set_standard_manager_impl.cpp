#include "boost/filesystem.hpp"

#include "StandardResourceManager.hpp"
#include "StandardLogManager.hpp"
#include "StandardSceneManager.hpp"
#include "StandardInputManager.hpp"
#include "StandardPerformanceManager.hpp"
#include "StandardRenderingManager.hpp"
#include "StandardSoundManager.hpp"
#include "StandardSaveManager.hpp"
#include "StandardDebugManager.hpp"
#include "StandardConfigManager.hpp"
#include "SystemVariables.hpp"

namespace plnt {
	void game::set_standard_managers() const {
		using namespace private_;
		set_resource_manager(std::make_shared<StandardResourceManager>());
		const auto log_mgr = std::make_shared<StandardLogManager>();
		//開発モードならコンソールウインドウを作成し、そこにログを出力
		if (system_variables::DevelopmentMode) {
			//コンソール出力を有効化
			log_mgr->ValidateConsoleOutPut();
		}
		//ログディレクトリがなかったら作る
		if (!boost::filesystem::exists(system_variables::file_system::LogDirectory)) {
			boost::filesystem::create_directory(system_variables::file_system::LogDirectory);
		}
		//ログ出力ファイルを有効にする
		log_mgr->ValidateFileOutPut(
			system_variables::file_system::LogDirectory + "\\" + system_variables::file_system::LogOutPutFileName);
		set_log_manager(log_mgr);
		set_scene_manager(std::make_shared<StandardSceneManager>());
		set_input_manager(std::make_shared<StandardInputManager>());
		set_performance_manager(std::make_shared<StandardPerformanceManager>());
		set_rendering_manager(std::make_shared<StandardRenderingManager>());
		set_sound_manager(std::make_shared<StandardSoundManager>());
		set_save_manager(std::make_shared<StandardSaveManager>());
		set_debug_manager(std::make_shared<StandardDebugManager>());
		set_config_manager(std::make_shared<standard_config_manager>());
	}
}
