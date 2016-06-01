#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	class GameObjectManagerPublicInterface {
	public:
		virtual ~GameObjectManagerPublicInterface() = default;
		/*ゲームオブジェクトを作成*/
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id) = 0;
		/*名前をつけてゲームオブジェクトを作成*/
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id, const std::string& name) = 0;
	};
}