#include "SceneSetUpper.hpp"
#include "Scene.hpp"
#include "log_utility.hpp"
#include "SceneSetUpProxy.hpp"

namespace plnt {
	bool SceneSetUpper::InitializeScene(private_::Scene &scene, const util::parameter_holder &initialize_parameters) {
		SceneSetUpProxy safs(scene);
		//固有設定
		if (!SetUpScene(safs, initialize_parameters)) {
			PE_LOG_ERROR("シーンの固有設定に失敗しました。");
			return false;
		}
		return true;
	}

	util::parameter_holder SceneSetUpper::TerminateScene(private_::Scene &scene, const std::string &next_scene_id,
	                                                    const util::parameter_holder &finalize_parameters) {
		SceneSetUpProxy safs(scene);
		auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //固有終了処理
		return ret;
	}
}
