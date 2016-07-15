#pragma once

#include <string>

namespace planeta {
	namespace core {
		/*�ړ����ŗp����I�u�W�F�N�g�̃J�e�S��*/
		enum class ObjectCategory {
			GameObject,
			GameObjectComponent,
			GameObjectInterface,
			Task,
			Resource,
			EventArg,
			Scene,
			StandardInterface,
			Others
		};
		/*�ړ������������珜������*/
		std::string RemovePrefix(const std::string& class_name);
		/*�ړ����ɂ��I�u�W�F�N�g�̃J�e�S���𔻒f����*/
		ObjectCategory DetectCategoryByPrefix(const std::string& class_name);
		/*�ړ�����ǉ�����*/
		std::string AddPrefix(const std::string& class_id,ObjectCategory category);
	}
}