#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace plnt {
	class file_manipulator;

	namespace private_ {
		class resource_manager;
		class config_manager;

		namespace init_funcs {
			//PlanetaEngineのInit関数内で呼ばれる。戻り値に初期化の成否と、終了処理関数を返す。エラーメッセージはこれらの関数内で出力する。
			//DxLibの初期化
			std::tuple<bool, std::function<void()>> initialize_dx_lib(const config_manager &cfg_mgr);
			//Effekseerの初期化。DxLib初期化の後に行う
			std::tuple<bool, std::function<void()>> initialize_effekseer();
			//リソースシステムの初期化
			std::tuple<bool, std::function<void()>> initialize_resource_system(
				resource_manager &mgr, const std::shared_ptr<file_manipulator> &res_dir_manipulator);
			//その他初期化補助関数
			bool load_config(config_manager &mgr, const std::shared_ptr<file_manipulator> &sys_dir_manipulator,
			                const std::shared_ptr<file_manipulator> &cfg_dir_manipulator);

			enum class file_accessor_kind { resource, save_data, system, config };

			std::shared_ptr<file_manipulator> create_file_manipulator(file_accessor_kind kind);
		}
	}
}
