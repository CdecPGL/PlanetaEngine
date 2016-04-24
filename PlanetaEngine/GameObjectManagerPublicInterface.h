#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	class GameObjectManagerPublicInterface {
	public:
		virtual ~GameObjectManagerPublicInterface() = default;
		/*ゲームオブジェクトを作成*/
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id) = 0;
		/*名前をつけてゲームオブジェクトを作成*/
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id, const std::string& name) = 0;
	};
}