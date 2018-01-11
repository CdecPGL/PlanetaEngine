#include "SceneSetUpper.hpp"
#include "Scene.hpp"
#include "LogUtility.hpp"
#include "SceneSetUpProxy.hpp"

namespace plnt {
	bool SceneSetUpper::InitializeScene(private_::Scene& scene, const util::ParameterHolder& initialize_parameters) {
		SceneSetUpProxy safs(scene);
		//固有設定
		if (!SetUpScene(safs, initialize_parameters)) {
			PE_LOG_ERROR("シーンの固有設定に失敗しました。");
			return false;
		}
		return true;
	}

	util::ParameterHolder SceneSetUpper::TerminateScene(private_::Scene& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) {
		SceneSetUpProxy safs(scene);
		auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //固有終了処理
		return ret;
	}
}