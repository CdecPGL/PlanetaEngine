#pragma once

namespace planeta_engine {
	class Camera;
	namespace game {
		class GameObjectManagerPublicInterface;
		class GUIManagerPublicInterface;
		class TaskManagerPublicInterface;
	}
	namespace core {
		class ScenePublicInterface {
		public:
			virtual ~ScenePublicInterface() = default;
			/*�v���Z�X�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::TaskManagerPublicInterface& game_process_manager() = 0;
			/*�Q�[���I�u�W�F�N�g�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::GameObjectManagerPublicInterface& game_object_manager() = 0;
			/*GUI�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::GUIManagerPublicInterface& gui_manager() = 0;
			/*�J�����̃C���X�^���X���擾*/
			virtual Camera& camera() = 0;
		};
	}
}