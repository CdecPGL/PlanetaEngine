#include "SceneSetUpper.h"
#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {
		bool SceneSetUpper::InitializeScene(Scene& scene,const utility::ParameterHolder& initialize_parameters) {
			SceneAccessorForSetUp safs(scene);
			//�V�[���C���^�[�t�F�C�X�����W���[���ɓo�^
			scene.RegisterSceneInterfaceToModules();
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
			SceneAccessorForSetUp safs(scene);
			auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //�ŗL�I������
			scene.Finalize(); //�I������
			return ret;
		}

	}
}