#pragma once

#include "GameObjectManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameObjectManagerAccessorForSetUp final {
		public:
			explicit GameObjectManagerAccessorForSetUp(GameObjectManagerPublicInterface& gopi) :game_object_manager_public_interface_(gopi) {};
			/*�Q�[���I�u�W�F�N�g���쐬*/
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper) { return game_object_manager_public_interface_.CreateGameObject(game_object_setupper); }
			/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) { return game_object_manager_public_interface_.CreateGameObject(game_object_setupper, name); };
			/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper) { return game_object_manager_public_interface_.CreateAndActivateGameObject(game_object_setupper); }
			/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) { return CreateAndActivateGameObject(game_object_setupper, name); }
		private:
			GameObjectManagerPublicInterface& game_object_manager_public_interface_;
		};
	}
}