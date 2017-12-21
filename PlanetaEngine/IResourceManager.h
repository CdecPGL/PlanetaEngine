#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include "LogUtility.h"

namespace planeta {
	class ResourceBase;
	/*! リソースマネージャ公開インターフェイス*/
	class IResourceManager {
	public:
		virtual ~IResourceManager() = default;
		/*! リソースをIDで取得*/
		virtual std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info& type_info, const std::string& id) = 0;
		/*! リソースをIDで取得し型を変換する*/
		template<class RT>
		std::shared_ptr<RT> GetResourceByID(const std::string& id) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = GetResourceByTypeAndID(typeid(RT), id);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) {
					return out;
				}
				else {
					PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(RT).name(), "\")");
					return nullptr;
				}
			}
			else {
				return nullptr;
			}
		}
		/*リソースをFullIDで取得*/
		virtual std::shared_ptr<ResourceBase> GetResourceByFullID(const std::string& full_id) = 0;
	};
}
