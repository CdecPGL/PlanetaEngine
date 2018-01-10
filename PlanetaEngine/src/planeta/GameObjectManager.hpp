#pragma once

#include "planeta/IGameObjectManager.hpp"
#include "SceneModule.hpp"

namespace plnt {
	namespace private_ {
		class GameObjectManager : public IGameObjectManager, public SceneModule {
		public:
			virtual ~GameObjectManager() = 0 {};
			/*ゲームオブジェクト登録解除(終了処理を行う)*/
			virtual bool RemoveGameObject(int id) = 0;
			/*有効化*/
			virtual bool ActivateGameObject(int id) = 0;
			/*無効化*/
			virtual bool InActivateGameObject(int id) = 0;
			/*すべてのゲームオブジェクトを破棄*/
			virtual void RemoveAllGameObjects() = 0;
		};
	}
}