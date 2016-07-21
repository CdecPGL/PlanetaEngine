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
	/*! @brief ���t���N�V�����V�X�e��
		
		���t���N�V�����V�X�e���ɐÓI�֐��ŃA�N�Z�X�ł���N���X�B���݂͌^�̓o�^�ƁA�쐬�ɂ̂ݑΉ��B
	*/
	class Reflection : private util::NonCopyable<Reflection> {
	public:
		/*�I�u�W�F�N�g��o�^*/
		template<class T>
		static void RegisterObject(const std::string& object_type_id, const std::function<std::shared_ptr<core::Object>()>& creator) {
			static_assert(std::is_base_of<core::Object, T>::value == true, "T must derive Object.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			RegisterObject_(typeid(T), object_type_id, creator);
		}
		//! ObjectTypeID�ŃI�u�W�F�N�g���쐬
		static std::shared_ptr<core::Object> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept;
		/*! @brief ObjectTypeID�ŃI�u�W�F�N�g���쐬���Č^�ϊ�
			@return �ϊ���̃|�C���^��Ԃ��B���݂��Ȃ��ꍇ��nullptr��Ԃ��B
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept {
			auto ptr = CreateObjectByObjectTypeID(object_type_id);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("�w��^�ɕϊ��ł��܂���ł����B(ObjectTypeID:", object_type_id, ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! �^���ŃI�u�W�F�N�g���쐬
		static std::shared_ptr<core::Object> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept;
		/*! @brief �^���ŃI�u�W�F�N�g���쐬���Č^�ϊ�
			@return �ϊ���̃|�C���^��Ԃ��B���݂��Ȃ��ꍇ��nullptr��Ԃ��B
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept {
			auto ptr = CreateObjectByStdTypeInfo(t_info);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("�w��^�ɕϊ��ł��܂���ł����B(type_indo:", t_info.name(), ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! �^����ObjectTypeID���擾����
		template<typename T>
		static std::string GetObjectTypeIDByType() {
			return GetObjectTypeIDByStdTypeInfo(typeid(T));
		}
		//! std::type_info����ObjectTypeID���擾����
		static std::string GetObjectTypeIDByStdTypeInfo(const std::type_info& tinfo);
		//! ObjectTypeID����^�����擾����
		static const std::type_info& GetStdTypeInfoByObjectTypeID(const std::string& id);
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

/*! @def
	Object���V�X�e���ɓo�^����
	@param type �^
	@param base_check �p������ׂ��^
*/
#define PE_REGISTER_OBJECT(type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (#type,[]{return std::make_shared<type>();});}
/*! @def
	ID���w�肵��Object���V�X�e���ɓo�^����
	@param ID(�����񃊃e����)
	@param base_check �p������ׂ��^
*/
#define PE_REGISTER_OBJECT_WITH_ID(id,type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (id,[]{return std::make_shared<type>();});}