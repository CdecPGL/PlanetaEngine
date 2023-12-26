#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace plnt {
	class FileManipulator;

	namespace private_ {
		class ResourceManager;
		class ConfigManager;

		namespace init_funcs {
			//PlanetaEngineのInit関数内で呼ばれる。戻り値に初期化の成否と、終了処理関数を返す。エラーメッセージはこれらの関数内で出力する。
			//DxLibの初期化
			std::tuple<bool, std::function<void()>> InitializeDxLib(ConfigManager &cfg_mgr);
			//Effekseerの初期化。DxLib初期化の後に行う
			std::tuple<bool, std::function<void()>> InitializeEffekseer();
			//リソースシステムの初期化
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(
				ResourceManager &mgr, const std::shared_ptr<FileManipulator> &resource_dir_manipurator);
			//その他初期化補助関数
			bool LoadConfig(ConfigManager &mgr, const std::shared_ptr<FileManipulator> &sys_dir_manipurator,
			                const std::shared_ptr<FileManipulator> &cfg_dir_manipurator);

			enum class FileAccessorKind { Resource, SaveData, System, Config };

			std::shared_ptr<FileManipulator> CreateFileManipurator(FileAccessorKind kind);
		}
	}
}
