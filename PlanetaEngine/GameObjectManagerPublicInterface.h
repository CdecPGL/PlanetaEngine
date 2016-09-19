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
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id) = 0;
		/*名前をつけてゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::string& name) = 0;
		/*外部データの読み込みを行わないでゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id) = 0;
		/*外部データの読み込みを行わないで名前をつけてゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id, const std::string& name) = 0;
	};
}