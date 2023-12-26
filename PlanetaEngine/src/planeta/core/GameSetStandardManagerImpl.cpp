﻿#include "boost/filesystem.hpp"

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
	void Game::SetStandardManagers() {
		using namespace private_;
		SetResourceManager(std::make_shared<StandardResourceManager>());
		auto log_mgr = std::make_shared<StandardLogManager>();
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
		SetLogManager(log_mgr);
		SetSceneManager(std::make_shared<private_::StandardSceneManager>());
		SetInputManager(std::make_shared<private_::StandardInputManager>());
		SetPerformanceManager(std::make_shared<private_::StandardPerformanceManager>());
		SetRenderingManager(std::make_shared<private_::StandardRenderingManager>());
		SetSoundManager(std::make_shared<private_::StandardSoundManager>());
		SetSaveManager(std::make_shared<private_::StandardSaveManager>());
		SetDebugManager(std::make_shared<private_::StandardDebugManager>());
		SetConfigManager(std::make_shared<private_::StandardConfigManager>());
	}
}
