#include "SAImplGameObjectCreateFunctions.h"
#include "ScenePublicInterface.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObjectAccessor.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameObjectCreateFunctions::SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi) :gompi_(spi.game_object_manager()) {}

		/*ゲームオブジェクトを作成*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(const std::string& game_object_create_id) {
			return gompi_.CreateGameObject(game_object_create_id); 
		}
		/*ゲームオブジェクトを名前を付けて作成*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(const std::string& game_object_create_id, const std::string& name) { 
			return gompi_.CreateGameObject(game_object_create_id, name);
		};
		/*ゲームオブジェクトを作成して有効化*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(const std::string& game_object_create_id) {
			auto go = CreateGameObject(game_object_create_id);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}
		/*ゲームオブジェクトを作成して名前を付けて有効化*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(const std::string& game_object_create_id, const std::string& name) {
			auto go = CreateGameObject(game_object_create_id, name);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}
	}
}