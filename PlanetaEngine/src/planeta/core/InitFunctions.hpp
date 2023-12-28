#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace plnt {
	class file_manipulator;

	namespace private_ {
		class ResourceManager;
		class config_manager;

		namespace init_funcs {
			//PlanetaEngineのInit関数内で呼ばれる。戻り値に初期化の成否と、終了処理関数を返す。エラーメッセージはこれらの関数内で出力する。
			//DxLibの初期化
			std::tuple<bool, std::function<void()>> InitializeDxLib(config_manager &cfg_mgr);
			//Effekseerの初期化。DxLib初期化の後に行う
			std::tuple<bool, std::function<void()>> InitializeEffekseer();
			//リソースシステムの初期化
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(
				ResourceManager &mgr, const std::shared_ptr<file_manipulator> &resource_dir_manipurator);
			//その他初期化補助関数
			bool LoadConfig(config_manager &mgr, const std::shared_ptr<file_manipulator> &sys_dir_manipurator,
			                const std::shared_ptr<file_manipulator> &cfg_dir_manipurator);

			enum class FileAccessorKind { Resource, SaveData, System, Config };

			std::shared_ptr<file_manipulator> CreateFileManipurator(FileAccessorKind kind);
		}
	}
}
