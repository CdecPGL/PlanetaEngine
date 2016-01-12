#include "SAImplGameObjectCreateFunctions.h"
#include "ScenePublicInterface.h"
#include "GameObjectManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameObjectCreateFunctions::SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi) :gompi_(spi.game_object_manager()) {}

		/*�Q�[���I�u�W�F�N�g���쐬*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper& game_object_setupper) { return gompi_.CreateGameObject(game_object_setupper); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper&& game_object_setupper) { return gompi_.CreateGameObject(game_object_setupper); }
		/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name) { return gompi_.CreateGameObject(game_object_setupper, name); };
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name) { return gompi_.CreateGameObject(game_object_setupper, name); };
		/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper) { return gompi_.CreateAndActivateGameObject(game_object_setupper); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper) { return gompi_.CreateAndActivateGameObject(game_object_setupper); }
		/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
		utility::WeakPointer<game::IGameObjectAccessor> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(game::GameObjectSetUpper&& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
	}
}