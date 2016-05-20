#include "SceneSetUpper.h"
#include "Scene.h"
#include "SystemLog.h"
#include "SceneSystemSetUpper.h"

namespace planeta_engine {
	namespace core {
		bool SceneSetUpper::InitializeScene(Scene& scene,const utility::ParameterHolder& initialize_parameters) {
			SceneSetUpProxy safs(scene);
			//�V�[���f�[�^�̏���
			scene.PrepareSceneData();
			//�V�X�e���ݒ�(����v���Z�X�̍쐬��V�[���f�[�^�̍X�V)
			if (!SceneSystemSetUpper()(scene)) {
				debug::SystemLog::instance().LogError("�V�[���̃V�X�e���ݒ�Ɏ��s���܂����B",__FUNCTION__);
				return false;
			}
			//�V�[���f�[�^�����W���[���ɓo�^
			scene.RegisterSceneDataToModules();
			//�ŗL�ݒ�
			if (!SetUpScene(safs, initialize_parameters)) {
				debug::SystemLog::instance().LogError("�V�[���̌ŗL�ݒ�Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			//������
			if (!scene.Initialize()) {
				debug::SystemLog::instance().LogError("�V�[���̏������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			return true;
		}

		utility::ParameterHolder SceneSetUpper::TerminateScene(Scene& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) {
			SceneSetUpProxy safs(scene);
			auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //�ŗL�I������
			scene.Finalize(); //�I������
			return ret;
		}

	}
}