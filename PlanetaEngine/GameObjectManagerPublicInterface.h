#pragma once

#include <string>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
		class GameObjectSetUpper;
		class GameObjectManagerPublicInterface{
		public:
			virtual ~GameObjectManagerPublicInterface() = default;
			/*�Q�[���I�u�W�F�N�g���쐬*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id) = 0;
			/*���O�����ăQ�[���I�u�W�F�N�g���쐬*/
			virtual utility::WeakPointer<IGameObjectAccessor> CreateGameObject(const std::string& game_object_create_id,const std::string& name) = 0;
			/*�Q�[���I�u�W�F�N�g�Z�b�g�A�b�p�[��o�^*/
			virtual void RegisterGameObjectSetUpper(const std::string& game_object_create_id, std::shared_ptr<GameObjectSetUpper>&& game_object_setupper) = 0;
		};
	}
}