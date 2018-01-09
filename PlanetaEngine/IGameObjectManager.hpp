#pragma once

#include <string>
#include <functional>
#include <vector>
#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	class IGameObjectManager {
	public:
		virtual ~IGameObjectManager() = default;
		/*��`�t�@�C�����w�肵�ăQ�[���I�u�W�F�N�g���쐬*/
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id) = 0;
		/*��`�t�@�C�����w�肵�Ė��O�t���Q�[���I�u�W�F�N�g���쐬*/
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id, const std::string& name) = 0;
		/*�R���|�[�l���g�^�C�vID���w�肵�ăQ�[���I�u�W�F�N�g���쐬*/
		virtual WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list) = 0;
		/*�R���|�[�l���g�^�C�vID���w�肵�Ė��O�t���Q�[���I�u�W�F�N�g���쐬*/
		virtual WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list, const std::string& name) = 0;
	};
}