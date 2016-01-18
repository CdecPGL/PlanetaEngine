#pragma once

#include <string>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
		class GameObjectSetUpper;
		class GameObjectManagerPublicInterface{
		public:
			virtual ~GameObjectManagerPublicInterface() = default;
			/*ゲームオブジェクトを作成*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id) = 0;
			/*名前をつけてゲームオブジェクトを作成*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id,const std::string& name) = 0;
			/*ゲームオブジェクトセットアッパーを登録*/
			virtual void RegisterGameObjectSetUpper(const std::string& game_object_create_id, std::shared_ptr<GameObjectSetUpper>&& game_object_setupper) = 0;
		};
	}
}