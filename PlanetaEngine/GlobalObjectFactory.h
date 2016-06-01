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
		//オブジェクトファクトリ(シングルトンクラス)
		class GlobalObjectFactory : private util::NonCopyable<GlobalObjectFactory> {
		public:
			/*インスタンスを取得*/
			static GlobalObjectFactory& instance();
			/*オブジェクトを登録*/
			template<class T>
			void RegisterObject(const std::string& id, const std::function<std::shared_ptr<Object>()>& creator) {
				static_assert(std::is_base_of<Object, T>::value == true, "T must derive Object.");
				//PE_LOG_MESSAGE("Object is registered.", id);
				id_creator_map_.emplace(id, creator);
				type_creator_map_.emplace(typeid(T), creator);
			}
			/*ID(型名)でオブジェクトを作成*/
			std::shared_ptr<Object> CreateObjectByID(const std::string& id);
			template<typename T>
			std::shared_ptr<T> CreateObjectByID(const std::string& id) {
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
			std::shared_ptr<Object> CreateObjectByTypeInfo(const std::type_info& t_info);
		private:
			GlobalObjectFactory();

			std::unordered_map<std::string, std::function<std::shared_ptr<Object>()>> id_creator_map_;
			std::unordered_map<std::type_index, std::function<std::shared_ptr<Object>()>> type_creator_map_;
		};
	}
}