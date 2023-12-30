#include "scene_setupper.hpp"
#include "scene.hpp"
#include "log_utility.hpp"
#include "scene_setup_proxy.hpp"

namespace plnt {
	bool scene_setupper::initialize_scene(private_::scene &scene, const util::parameter_holder &initialize_parameters) {
		//固有設定
		if (scene_setup_proxy setup_proxy(scene); !setup_scene(setup_proxy, initialize_parameters)) {
			PE_LOG_ERROR("シーンの固有設定に失敗しました。");
			return false;
		}
		return true;
	}

	util::parameter_holder scene_setupper::terminate_scene(private_::scene &scene, const std::string &next_scene_id,
	                                                      const util::parameter_holder &finalize_parameters) {
		scene_setup_proxy setup_proxy(scene);
		auto ret = finalize_scene(setup_proxy, next_scene_id, finalize_parameters); //固有終了処理
		return ret;
	}
}
