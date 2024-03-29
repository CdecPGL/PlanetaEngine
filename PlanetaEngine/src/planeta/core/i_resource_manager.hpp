﻿#pragma once

#include <string>
#include <memory>
#include <type_traits>

#include "log_utility.hpp"

namespace plnt {
	class resource_base;

	/// <summary>リソースマネージャ公開インターフェイス。</summary>
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_resource_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_resource_manager() = 0 {}

		/// <summary>リソースをIDで取得する。</summary>
		/// <param name="type_info">取得するリソースの型情報</param>
		/// <param name="id">取得するリソースのリソースID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		[[nodiscard]] virtual std::shared_ptr<resource_base> get_resource_by_type_and_id(const std::type_info &type_info,
		                                                                  const std::string &id) = 0;
		/// <summary>リソースをIDで取得し型を変換する。</summary>
		/// <param name="id">取得するリソースのリソースID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		template <class T>
		[[nodiscard]] std::shared_ptr<T> get_resource_by_id(const std::string &id) {
			static_assert(std::is_base_of_v<resource_base, T>, "T must derive ResourceBase");
			if (const auto rsc = get_resource_by_type_and_id(typeid(T), id)) {
				if (auto out = std::dynamic_pointer_cast<T>(rsc)) { return out; }
				PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(T).name(), "\")");
				return nullptr;
			}
			return nullptr;
		}

		/// <summary>リソースをFullIDで取得する。</summary>
		/// <param name="full_id">取得するリソースのリソースFullID</param>
		/// <returns>取得されたリソース。取得できなかった場合はnullptr</returns>
		[[nodiscard]] virtual std::shared_ptr<resource_base> get_resource_by_full_id(const std::string &full_id) = 0;
	};
}
