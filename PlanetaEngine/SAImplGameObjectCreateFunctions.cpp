#include "SAImplGameObjectCreateFunctions.h"
#include "ScenePublicInterface.h"
#include "GameObjectManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameObjectCreateFunctions::SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi) :gompi_(spi.game_object_manager()) {}

		/*ゲームオブジェクトを作成*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper& game_object_setupper) { return gompi_.CreateGameObject(game_object_setupper); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper&& game_object_setupper) { return gompi_.CreateGameObject(game_object_setupper); }
		/*ゲームオブジェクトを名前を付けて作成*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name) { return gompi_.CreateGameObject(game_object_setupper, name); };
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name) { return gompi_.CreateGameObject(game_object_setupper, name); };
		/*ゲームオブジェクトを作成して有効化*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper) { return gompi_.CreateAndActivateGameObject(game_object_setupper); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper) { return gompi_.CreateAndActivateGameObject(game_object_setupper); }
		/*ゲームオブジェクトを作成して名前を付けて有効化*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
	}
}