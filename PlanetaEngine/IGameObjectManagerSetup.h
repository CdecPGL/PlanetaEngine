#pragma once

#include <memory>
#include <string>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
		class GameObjectSetUpper;
		class IGameObjectManagerSetup {
		public:
			virtual ~IGameObjectManagerSetup() = 0 {};
			/*�Q�[���I�u�W�F�N�g���쐬*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper) = 0;
			/*�Q�[���I�u�W�F�N�g�𖼑O��t���č쐬*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name) = 0;
			/*�Q�[���I�u�W�F�N�g���쐬���ėL����*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper) = 0;
			/*�Q�[���I�u�W�F�N�g���쐬���Ė��O��t���ėL����*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name) = 0;
		};
	}
}
