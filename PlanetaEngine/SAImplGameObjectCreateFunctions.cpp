#include "SAImplGameObjectCreateFunctions.h"
#include "ScenePublicInterface.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameObjectCreateFunctions::SAImplGameObjectCreateFunctions(core::ScenePublicInterface& spi) :gompi_(spi.game_object_manager()) {}

		/*�Q�[���I�u�W�F�N�g���쐬*/
		utility::WeakPointer<IGameObject> SAImplGameObjectCreateFunctions::CreateGameObject(const std::string& game_object_create_id) {
			return gompi_.CreateGameObject(game_object_create_id); 
		}
		/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
		utility::WeakPointer<IGameObject> SAImplGameObjectCreateFunctions::CreateGameObjectWithName(const std::string& game_object_create_id, const std::string& name) { 
			return gompi_.CreateGameObject(game_object_create_id, name);
		};
		/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
		utility::WeakPointer<IGameObject> SAImplGameObjectCreateFunctions::CreateAndActivateGameObject(const std::string& game_object_create_id) {
			auto go = CreateGameObject(game_object_create_id);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}
		/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
		utility::WeakPointer<IGameObject> SAImplGameObjectCreateFunctions::CreateAndActivateGameObjectWithName(const std::string& game_object_create_id, const std::string& name) {
			auto go = CreateGameObjectWithName(game_object_create_id, name);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}
	}
}