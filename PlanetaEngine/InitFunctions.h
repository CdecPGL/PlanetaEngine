#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace planeta {
	class FileAccessor;
	namespace private_ {
		class ResourceManager;
		class FileSystemManager;
		class ConfigManager;
		namespace init_funcs {
			//PlanetaEngineのInit関数内で呼ばれる。戻り値に初期化の成否と、終了処理関数を返す。エラーメッセージはこれらの関数内で出力する。
			//DxLibの初期化
			std::tuple<bool,std::function<void()>> InitializeDxLib(ConfigManager& cfg_mgr);
			//Effekseerの初期化。DxLib初期化の後に行う
			std::tuple<bool, std::function<void()>> InitializeEffekseer();
			//リソースシステムの初期化
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(ResourceManager& mgr, const std::shared_ptr<FileAccessor> res_file_sccessor);
			//その他初期化補助関数
			bool LoadConfig(ConfigManager& mgr, const std::shared_ptr<FileAccessor>& sys_dir_accessor, const std::shared_ptr<FileAccessor>& cfg_dir_accessor);
			enum class FileAccessorKind{Resource,SaveData,System,Config};
			std::shared_ptr<FileAccessor> CreateFileAccessor(FileSystemManager& mgr, FileAccessorKind kind);
		}
	}
}