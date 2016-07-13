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
	//リフレクションシステム(スタティッククラス)
	class Reflection : private util::NonCopyable<Reflection> {
	public:
		/*オブジェクトを登録*/
		template<class T>
		static void RegisterObject(const std::string& id, const std::function<std::shared_ptr<core::Object>()>& creator) {
			static_assert(std::is_base_of<core::Object, T>::value == true, "T must derive Object.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			type_id_map_.emplace(typeid(T), id);
			id_creator_map_.emplace(id, creator);
			type_creator_map_.emplace(typeid(T), creator);
		}
		/*ID(型名)でオブジェクトを作成*/
		static std::shared_ptr<core::Object> CreateObjectByID(const std::string& id);
		template<typename T>
		static std::shared_ptr<T> CreateObjectByID(const std::string& id) {
			auto ptr = CreateObjectByID(id);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("指定型に変換できませんでした。(ObjectID:", id, ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		/*型情報でオブジェクトを作成*/
		static std::shared_ptr<core::Object> CreateObjectByTypeInfo(const std::type_info& t_info);
		/*型から型IDを取得*/
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

//Objectをシステムに登録する(型,継承すべき型)
#define PE_REGISTER_OBJECT(type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (#type,[]{return std::make_shared<type>();});}

//IDを指定してObjectをシステムに登録する(ID<文字列リテラル>,型,継承すべき型)
#define PE_REGISTER_OBJECT_WITH_ID(id,type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (id,[]{return std::make_shared<type>();});}