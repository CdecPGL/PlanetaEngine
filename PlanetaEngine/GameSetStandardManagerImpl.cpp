#include "boost/filesystem.hpp"

#include "PlanetaEngine.h"

#include "StandardResourceManager.h"
#include "StandardLogManager.h"
#include "StandardFileSystemManager.h"
#include "StandardSceneManager.h"
#include "StandardInputManager.h"

#include "SystemVariables.h"

namespace planeta {
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
		log_mgr->ValidateFileOutPut(system_variables::file_system::LogDirectory + "\\" + system_variables::file_system::LogOutPutFileName);
		SetLogManager(log_mgr);
		SetFileSystemManager(std::make_shared<private_::StandardFileSystemManager>());
		SetSceneManager(std::make_shared<private_::StandardSceneManager>());
		SetInputManager(std::make_shared<private_::StandardInputManager>());
	}
}