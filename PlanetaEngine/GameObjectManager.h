#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	class GameObjectBase;
	class IGameObject;
	namespace core {
		struct SceneData;
	}
	class GameObjectManager final : public core::SceneModule
		, public GameObjectManagerPublicInterface, private utility::NonCopyable<GameObjectManager> {
	public:
		GameObjectManager();
		~GameObjectManager();
		//���[�U�A�N�Z�X�\�֐�
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id)override;
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id, const std::string& name)override;

		//�V�X�e���֐�
		/*������*/
		bool Initialize()override;
		/*�I������*/
		void Finalize()override;
		/*�V�[���f�[�^�Z�b�g*/
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data)override;

		/*�Ǘ�����*/
		void Update()override;
		/*�Q�[���I�u�W�F�N�g�o�^����(�I���������s��)*/
		bool RemoveGameObject(int id);
		/*�L����*/
		bool ActivateGameObject(int id);
		/*������*/
		bool InActivateGameObject(int id);
		void TakeOver(const GameObjectManager& gom); //�Q�[���I�u�W�F�N�g�̈��p������(������)
		/*���ׂẴQ�[���I�u�W�F�N�g��j��*/
		void RemoveAllGameObjects();
	private:
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
		std::unordered_map<std::string, int> name_id_map_;
		std::vector<std::shared_ptr<GameObjectBase>> garbage_;
		void RemoveProc_();
		int _id_counter;
		utility::WeakPointer<core::SceneData> scene_data_;
		static std::shared_ptr<GameObjectBase >CreateGameObjectByID_(const std::string& id);
		/*�Q�[���I�u�W�F�N�g�o�^(���������s���AID��Ԃ�)*/
		int RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go);
		int RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name);
	};
}