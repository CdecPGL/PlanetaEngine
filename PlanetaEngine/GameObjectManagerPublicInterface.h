#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	class GameObjectManagerPublicInterface {
	public:
		virtual ~GameObjectManagerPublicInterface() = default;
		/*�Q�[���I�u�W�F�N�g���쐬*/
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id) = 0;
		/*���O�����ăQ�[���I�u�W�F�N�g���쐬*/
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id, const std::string& name) = 0;
	};
}