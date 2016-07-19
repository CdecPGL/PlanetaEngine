#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	class SceneAccessorForGameObject;
	class GameObjectBase;
	class IGameObject;
	namespace core {
		struct SceneData;
		class GameObjectTemplateHolder;
	}
	class GameObjectManager final : public core::SceneModule
		, public GameObjectManagerPublicInterface, private util::NonCopyable<GameObjectManager> {
	public:
		GameObjectManager();
		~GameObjectManager();
		//���[�U�A�N�Z�X�\�֐�
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id)override;
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::string& name)override;
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id)override;
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id, const std::string& name)override;

		//�V�X�e���֐�
		/*������*/
		bool Initialize()override;
		/*�I������*/
		void Finalize()override;
		/*�V�[���f�[�^�Z�b�g*/
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data)override;

		/*�Ǘ�����*/
		void Update()override;
		/*�Q�[���I�u�W�F�N�g�o�^����(�I���������s��)*/
		bool RemoveGameObject(int id);
		/*�L����*/
		bool ActivateGameObject(int id);
		/*������*/
		bool InActivateGameObject(int id);
		/*���ׂẴQ�[���I�u�W�F�N�g��j��*/
		void RemoveAllGameObjects();
	private:
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
		std::unordered_map<std::string, int> name_id_map_;
		std::vector<std::shared_ptr<GameObjectBase>> garbage_;
		void RemoveProc_();
		int _id_counter;
		util::WeakPointer<core::SceneData> scene_data_;
		
		/*�Q�[���I�u�W�F�N�g�e���v���[�g�z���_�[*/
		std::unique_ptr<core::GameObjectTemplateHolder> game_object_template_holder_;

		/*�Q�[���I�u�W�F�N�g���쐬���A�Z�b�g�A�b�v���s��(�쐬�A�V�[���f�[�^�̃Z�b�g���s��)*/
		std::shared_ptr<GameObjectBase> CreateAndSetUpGameObject_(const std::string& game_object_type_id, const std::string& resource_id);

		/*�Q�[���I�u�W�F�N�g�o�^(�o�^�A�}�l�[�W���R�l�N�V�����ݒ�A���������s���AID��Ԃ�)*/
		int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go);
		int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name);
	};
}