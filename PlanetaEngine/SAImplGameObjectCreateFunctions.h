#pragma once

#include <string>
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
			/*�Q�[���I�u�W�F�N�g���쐬*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id);
			/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id, const std::string& name);
			/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(const std::string& game_object_create_id);
			/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
			utility::WeakPointer<game::IGameObjectAccessor> CreateAndActivateGameObject(const std::string& game_object_create_id, const std::string& name);
		private:
			game::GameObjectManagerPublicInterface& gompi_;
		};
	}
}