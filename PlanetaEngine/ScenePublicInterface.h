#pragma once

namespace planeta_engine {
	class Camera;
	namespace game {
		class GameObjectManagerPublicInterface;
		class UIManagerPublicInterface;
		class GameProcessManagerPublicInterface;
	}
	namespace core {
		class ScenePublicInterface {
		public:
			virtual ~ScenePublicInterface() = default;
			/*�v���Z�X�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::GameProcessManagerPublicInterface& game_process_manager() = 0;
			/*�Q�[���I�u�W�F�N�g�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::GameObjectManagerPublicInterface& game_object_manager() = 0;
			/*UI�}�l�[�W���̃C���X�^���X���擾*/
			virtual game::UIManagerPublicInterface& ui_manager() = 0;
			/*�J�����̃C���X�^���X�擾*/
			virtual Camera& camera() = 0;
		};
	}
}