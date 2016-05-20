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
			/*�Q�[���I�u�W�F�N�g���쐬*/
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id);
			/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
			utility::WeakPointer<IGameObject> CreateGameObjectWithName(const std::string& game_object_create_id, const std::string& name);
			/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
			utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& game_object_create_id);
			/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
			utility::WeakPointer<IGameObject> CreateAndActivateGameObjectWithName(const std::string& game_object_create_id, const std::string& name);
		private:
			GameObjectManagerPublicInterface& gompi_;
		};
	}
}