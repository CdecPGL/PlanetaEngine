#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
		class GameObjectSetUpper;
		class GameObjectManagerPublicInterface{
		public:
			virtual ~GameObjectManagerPublicInterface() = default;
			/*ゲームオブジェクトを作成*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper) = 0;
			/*ゲームオブジェクトを名前を付けて作成*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) = 0;
			/*ゲームオブジェクトを作成して有効化*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper) = 0;
			/*ゲームオブジェクトを作成して名前を付けて有効化*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) = 0;
		};
	}
}