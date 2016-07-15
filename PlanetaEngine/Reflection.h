#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>
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
			RegisterObject_(typeid(T), id, creator);
		}
		/*ID(�^��)�ŃI�u�W�F�N�g���쐬*/
		static std::shared_ptr<core::Object> CreateObjectByID(const std::string& id)noexcept;
		template<typename T>
		static std::shared_ptr<T> CreateObjectByID(const std::string& id)noexcept {
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
		static std::shared_ptr<core::Object> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept;
		/*�^����^ID���擾*/
		template<typename T>
		static std::string GetTypeIDByType() {
			return GetTypeIDByStdTypeInfo(typeid(T));
		}
		/*std::type_info����^ID���擾*/
		static std::string GetTypeIDByStdTypeInfo(const std::type_info& tinfo);
		/*�^ID����std::type_info���擾*/
		const std::type_info& GetStdTypeInfoByTypeID(const std::string& id);
	private:
		Reflection();
		class Impl_;
		static Impl_& impl_();
		static void RegisterObject_(const std::type_info& tinfo ,const std::string& id, const std::function<std::shared_ptr<core::Object>()>& creator);
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