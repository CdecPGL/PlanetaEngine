#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	namespace game {
		class GameObjectManagerPublicInterface{
		public:
			virtual ~GameObjectManagerPublicInterface() = default;
			/*�Q�[���I�u�W�F�N�g���쐬*/
			virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id) = 0;
			/*���O�����ăQ�[���I�u�W�F�N�g���쐬*/
			virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id,const std::string& name) = 0;
			/*�Q�[���I�u�W�F�N�g��o�^*/
			virtual void RegisterGameObjectCreator(const std::string& game_object_create_id, std::function<std::shared_ptr<GameObjectBase>()>&& creator) = 0;
			/*�Q�[���I�u�W�F�N�g���^�œo�^*/
			template<class GOT>
			virtual void RegisterGameObjectCreator(const std::string& game_object_create_id) {
				RegisterGameObjectCreator(game_object_create_id, []() {return GameObjectBase::Create<GOT>(); });
			}
		};
	}
}