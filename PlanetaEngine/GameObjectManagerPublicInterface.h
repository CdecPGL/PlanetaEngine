#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	namespace game {
		class GameObjectManagerPublicInterface{
		public:
			virtual ~GameObjectManagerPublicInterface() = default;
			/*ゲームオブジェクトを作成*/
			virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id) = 0;
			/*名前をつけてゲームオブジェクトを作成*/
			virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id,const std::string& name) = 0;
			/*ゲームオブジェクトを登録*/
			virtual void RegisterGameObjectCreator(const std::string& game_object_create_id, std::function<std::shared_ptr<GameObjectBase>()>&& creator) = 0;
			/*ゲームオブジェクトを型で登録*/
			template<class GOT>
			virtual void RegisterGameObjectCreator(const std::string& game_object_create_id) {
				RegisterGameObjectCreator(game_object_create_id, []() {return GameObjectBase::Create<GOT>(); });
			}
		};
	}
}