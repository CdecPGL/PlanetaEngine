#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <typeindex>
#include <cassert>
#include "NonCopyable.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		class Object;
		//�I�u�W�F�N�g�t�@�N�g��(�V���O���g���N���X)
		class GlobalObjectFactory : private util::NonCopyable<GlobalObjectFactory> {
		public:
			/*�C���X�^���X���擾*/
			static GlobalObjectFactory& instance();
			/*�I�u�W�F�N�g��o�^*/
			template<class T>
			void RegisterObject(const std::string& id, const std::function<std::shared_ptr<Object>()>& creator) {
				static_assert(std::is_base_of<Object, T>::value == true, "T must derive Object.");
				//PE_LOG_MESSAGE("Object is registered.", id);
				id_creator_map_.emplace(id, creator);
				type_creator_map_.emplace(typeid(T), creator);
			}
			/*ID(�^��)�ŃI�u�W�F�N�g���쐬*/
			std::shared_ptr<Object> CreateObjectByID(const std::string& id);
			template<typename T>
			std::shared_ptr<T> CreateObjectByID(const std::string& id) {
				auto ptr = CreateObjectByID(id);
				if (ptr == nullptr) { return nullptr; }
				auto tptr = std::dynamic_pointer_cast<T>(ptr);
				if (tptr == nullptr) {
					PE_LOG_ERROR("�w��^�ɕϊ��ł��܂���ł����B(ObjectID:", id, ",TargetType:", typeid(T).name(), ")");
					return nullptr;
				}
				return tptr;

			}
			/*�^���ŃI�u�W�F�N�g���쐬*/
			std::shared_ptr<Object> CreateObjectByTypeInfo(const std::type_info& t_info);
		private:
			GlobalObjectFactory();

			std::unordered_map<std::string, std::function<std::shared_ptr<Object>()>> id_creator_map_;
			std::unordered_map<std::type_index, std::function<std::shared_ptr<Object>()>> type_creator_map_;
		};
	}
}