#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Object.h"
#include "SceneAccessorForSetUp.h"
#include "ScenePublicInterface.h"
#include "WeakPointer.h"
#include "SharedPointerInstance.h"

#include "GameObjectManager.h"
#include "GameProcessManager.h"
#include "GUIManager.h"
#include "Camera.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class Screen;
		struct SceneData;
		class Scene : public Object,public utility::SharedPointerInstance<Scene>
			,public ScenePublicInterface{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*�V�[���̏�����*/
			bool Initialize();
			/*�V�[���̏I������*/
			bool Finalize();
			/*�V�[���̍X�V*/
			void Update();
			/*�v���Z�X�}�l�[�W���̃C���X�^���X���擾*/
			game::GameProcessManager& game_process_manager()override { assert(game_process_manager_ != nullptr); return *game_process_manager_; }
			/*�Q�[���I�u�W�F�N�g�}�l�[�W���̃C���X�^���X���擾*/
			game::GameObjectManager& game_object_manager()override { assert(game_object_manager_ != nullptr); return *game_object_manager_; }
			/*UI�}�l�[�W���̃C���X�^���X���擾*/
			game::GUIManager& gui_manager()override { assert(ui_manager_ != nullptr); return *ui_manager_; }
			/*�Q�[���N���X�ւ̃A�N�Z�X*/
			IGameAccessor& game_accessor() { return game_; }

			/*�ݒ�֐�*/
			SceneData& RefSceneData() { return *scene_data_; }
		private:
			Scene(const Scene&) = delete;
			Scene(Scene&&) = delete;
			Scene& operator=(const Scene&) = delete;
			Scene& operator=(Scene&&) = delete;

			IGameAccessor& game_;
			/*�V�[�����W���[��*/
			std::unique_ptr<game::GameProcessManager> game_process_manager_; //�Q�[���v���Z�X�}�l�[�W��
			std::unique_ptr<game::GameObjectManager> game_object_manager_; //�Q�[���I�u�W�F�N�g�}�l�[�W��
			std::unique_ptr<game::GUIManager> ui_manager_; //GUI�}�l�[�W��
			bool ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����

			std::unique_ptr<SceneData> scene_data_; //�V�[���f�[�^
		};
	}
}