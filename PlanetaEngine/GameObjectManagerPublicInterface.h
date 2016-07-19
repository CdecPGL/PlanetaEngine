#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	class GameObjectManagerPublicInterface {
	public:
		virtual ~GameObjectManagerPublicInterface() = default;
		/*�Q�[���I�u�W�F�N�g���쐬*/
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id) = 0;
		/*���O�����ăQ�[���I�u�W�F�N�g���쐬*/
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::string& name) = 0;
		/*�O���f�[�^�̓ǂݍ��݂��s��Ȃ��ŃQ�[���I�u�W�F�N�g���쐬*/
		virtual util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id) = 0;
		/*�O���f�[�^�̓ǂݍ��݂��s��Ȃ��Ŗ��O�����ăQ�[���I�u�W�F�N�g���쐬*/
		virtual util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id, const std::string& name) = 0;
	};
}