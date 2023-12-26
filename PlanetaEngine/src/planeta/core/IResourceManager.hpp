#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include "LogUtility.hpp"

namespace plnt {
	class ResourceBase;

	/// <summary>リソースマネージャ公開インターフェイス。</summary>
	class IResourceManager {
	public:
		/// <summary>デストラクタ。</summary>
		virtual ~IResourceManager() = default;
		/// <summary>リソースをIDで取得する。</summary>
		/// <param name="type_info">取得するリソースの型情報</param>
		/// <param name="id">取得するリソースのリソースID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		virtual std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info &type_info,
		                                                             const std::string &id) = 0;
		/// <summary>リソースをIDで取得し型を変換する。</summary>
		/// <param name="id">取得するリソースのリソースID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		template <class RT>
		std::shared_ptr<RT> GetResourceByID(const std::string &id) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = GetResourceByTypeAndID(typeid(RT), id);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) { return out; } else {
					PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(RT).name(), "\")");
					return nullptr;
				}
			} else { return nullptr; }
		}

		/// <summary>リソースをFullIDで取得する。</summary>
		/// <param name="full_id">取得するリソースのリソースFullID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		virtual std::shared_ptr<ResourceBase> GetResourceByFullID(const std::string &full_id) = 0;
	};
}
