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
	}
	//���t���N�V�����V�X�e��(�X�^�e�B�b�N�N���X)
	class Reflection : private util::NonCopyable<Reflection> {
	public:
		/*�I�u�W�F�N�g��o�^*/
		template<class T>
		static void RegisterObject(const std::string& id, const std::function<std::shared_ptr<core::Object>()>& creator) {
			static_assert(std::is_base_of<core::Object, T>::value == true, "T must derive Object.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			type_id_map_.emplace(typeid(T), id);
			id_creator_map_.emplace(id, creator);
			type_creator_map_.emplace(typeid(T), creator);
		}
		/*ID(�^��)�ŃI�u�W�F�N�g���쐬*/
		static std::shared_ptr<core::Object> CreateObjectByID(const std::string& id);
		template<typename T>
		static std::shared_ptr<T> CreateObjectByID(const std::string& id) {
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
		static std::shared_ptr<core::Object> CreateObjectByTypeInfo(const std::type_info& t_info);
		/*�^����^ID���擾*/
		template<typename T>
		static std::string GetTypeIDByType() {
			auto it = type_id_map_(typeid(T));
			return it == type_id_map_.end() ? ""
		}
	private:
		Reflection();
		static std::unordered_map<std::type_index, std::string> type_id_map_;
		static std::unordered_map<std::string, std::function<std::shared_ptr<core::Object>()>> id_creator_map_;
		static std::unordered_map<std::type_index, std::function<std::shared_ptr<core::Object>()>> type_creator_map_;
	};

	namespace core {
		template<typename ObjectType>
		class GlobalObjectRegisterer {
		public:
			GlobalObjectRegisterer(const char* object_id, const std::function<std::shared_ptr<Object>()>& creator) {
				Reflection::RegisterObject<ObjectType>(object_id, creator);
			}
		};
	}
}

//Object���V�X�e���ɓo�^����(�^,�p�����ׂ��^)
#define PE_REGISTER_OBJECT(type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (#type,[]{return std::make_shared<type>();});}

//ID���w�肵��Object���V�X�e���ɓo�^����(ID<�����񃊃e����>,�^,�p�����ׂ��^)
#define PE_REGISTER_OBJECT_WITH_ID(id,type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (id,[]{return std::make_shared<type>();});}