#include "SceneSetUpper.h"
#include "Scene.h"
#include "SystemLog.h"

namespace planeta_engine {
	bool SceneSetUpper::InitializeScene(core::Scene& scene, const util::ParameterHolder& initialize_parameters) {
		SceneSetUpProxy safs(scene);
		//�ŗL�ݒ�
		if (!SetUpScene(safs, initialize_parameters)) {
			PE_LOG_ERROR("�V�[���̌ŗL�ݒ�Ɏ��s���܂����B");
			return false;
		}
		return true;
	}

	util::ParameterHolder SceneSetUpper::TerminateScene(core::Scene& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) {
		SceneSetUpProxy safs(scene);
		auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //�ŗL�I������
		return ret;
	}
}