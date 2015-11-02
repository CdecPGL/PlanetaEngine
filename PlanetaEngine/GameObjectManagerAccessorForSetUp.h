#pragma once

#include "GameObjectManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameObjectManagerAccessorForSetUp final {
		public:
			explicit GameObjectManagerAccessorForSetUp(GameObjectManagerPublicInterface& gopi) :game_object_manager_public_interface_(gopi) {};
			/*ゲームオブジェクトを作成*/
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper) { return game_object_manager_public_interface_.CreateGameObject(game_object_setupper); }
			/*ゲームオブジェクトを名前を付けて作成*/
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) { return game_object_manager_public_interface_.CreateGameObject(game_object_setupper, name); };
			/*ゲームオブジェクトを作成して有効化*/
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper) { return game_object_manager_public_interface_.CreateAndActivateGameObject(game_object_setupper); }
			/*ゲームオブジェクトを作成して名前を付けて有効化*/
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
		private:
			GameObjectManagerPublicInterface& game_object_manager_public_interface_;
		};
	}
}