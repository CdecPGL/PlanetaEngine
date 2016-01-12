#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	namespace game {
		class IGameObjectAccessor;
		class GameObjectSetUpper;
		class GameObjectManagerPublicInterface;
	}
	namespace scene_accessor_impl {
		class SAImplGameObjectCreateFunctions {
		public: 
			explicit SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi);
			/*ゲームオブジェクトを作成*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(game::GameObjectSetUpper& game_object_setupper);
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(game::GameObjectSetUpper&& game_object_setupper);
			/*ゲームオブジェクトを名前を付けて作成*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name);
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name);
			/*ゲームオブジェクトを作成して有効化*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper);
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper);
			/*ゲームオブジェクトを作成して名前を付けて有効化*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name);
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name);

		private:
			game::GameObjectManagerPublicInterface& gompi_;
		};
	}
}