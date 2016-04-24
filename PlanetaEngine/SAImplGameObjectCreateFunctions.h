#pragma once

#include <string>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class IGameObject;
	class GameObjectManagerPublicInterface;
	namespace scene_accessor_impl {
		class SAImplGameObjectCreateFunctions {
		public: 
			explicit SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi);
			/*ゲームオブジェクトを作成*/
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id);
			/*ゲームオブジェクトを名前を付けて作成*/
			utility::WeakPointer<IGameObject> CreateGameObjectWithName(const std::string& game_object_create_id, const std::string& name);
			/*ゲームオブジェクトを作成して有効化*/
			utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& game_object_create_id);
			/*ゲームオブジェクトを作成して名前を付けて有効化*/
			utility::WeakPointer<IGameObject> CreateAndActivateGameObjectWithName(const std::string& game_object_create_id, const std::string& name);
		private:
			GameObjectManagerPublicInterface& gompi_;
		};
	}
}