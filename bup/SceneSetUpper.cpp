#include "SceneSetUpper.h"
#include "Scene.h"
#include "SystemLog.h"

namespace planeta {
	bool SceneSetUpper::InitializeScene(core::Scene& scene, const util::ParameterHolder& initialize_parameters) {
		SceneSetUpProxy safs(scene);
		//固有設定
		if (!SetUpScene(safs, initialize_parameters)) {
			PE_LOG_ERROR("シーンの固有設定に失敗しました。");
			return false;
		}
		return true;
	}

	util::ParameterHolder SceneSetUpper::TerminateScene(core::Scene& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) {
		SceneSetUpProxy safs(scene);
		auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //固有終了処理
		return ret;
	}
}