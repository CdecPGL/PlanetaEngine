#pragma once

#include "planeta/IGameObjectManager.hpp"
#include "SceneModule.hpp"

namespace plnt {
	namespace private_ {
		class GameObjectManager : public IGameObjectManager, public SceneModule {
		public:
			virtual ~GameObjectManager() = 0 {};
			/*�Q�[���I�u�W�F�N�g�o�^����(�I���������s��)*/
			virtual bool RemoveGameObject(int id) = 0;
			/*�L����*/
			virtual bool ActivateGameObject(int id) = 0;
			/*������*/
			virtual bool InActivateGameObject(int id) = 0;
			/*���ׂẴQ�[���I�u�W�F�N�g��j��*/
			virtual void RemoveAllGameObjects() = 0;
		};
	}
}