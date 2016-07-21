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
	/*! @brief リフレクションシステム
		
		リフレクションシステムに静的関数でアクセスできるクラス。現在は型の登録と、作成にのみ対応。
	*/
	class Reflection : private util::NonCopyable<Reflection> {
	public:
		/*オブジェクトを登録*/
		template<class T>
		static void RegisterObject(const std::string& object_type_id, const std::function<std::shared_ptr<core::Object>()>& creator) {
			static_assert(std::is_base_of<core::Object, T>::value == true, "T must derive Object.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			RegisterObject_(typeid(T), object_type_id, creator);
		}
		//! ObjectTypeIDでオブジェクトを作成
		static std::shared_ptr<core::Object> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept;
		/*! @brief ObjectTypeIDでオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept {
			auto ptr = CreateObjectByObjectTypeID(object_type_id);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("指定型に変換できませんでした。(ObjectTypeID:", object_type_id, ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! 型情報でオブジェクトを作成
		static std::shared_ptr<core::Object> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept;
		/*! @brief 型情報でオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept {
			auto ptr = CreateObjectByStdTypeInfo(t_info);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("指定型に変換できませんでした。(type_indo:", t_info.name(), ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! 型からObjectTypeIDを取得する
		template<typename T>
		static std::string GetObjectTypeIDByType() {
			return GetObjectTypeIDByStdTypeInfo(typeid(T));
		}
		//! std::type_infoからObjectTypeIDを取得する
		static std::string GetObjectTypeIDByStdTypeInfo(const std::type_info& tinfo);
		//! ObjectTypeIDから型情報を取得する
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
	Objectをシステムに登録する
	@param type 型
	@param base_check 継承するべき型
*/
#define PE_REGISTER_OBJECT(type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (#type,[]{return std::make_shared<type>();});}
/*! @def
	IDを指定してObjectをシステムに登録する
	@param ID(文字列リテラル)
	@param base_check 継承するべき型
*/
#define PE_REGISTER_OBJECT_WITH_ID(id,type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (id,[]{return std::make_shared<type>();});}