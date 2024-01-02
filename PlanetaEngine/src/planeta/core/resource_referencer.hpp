#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>

#include "log_utility.hpp"

namespace plnt {
	class resource_base;

	namespace private_ {
		class resource_manager_internal_accessor {
		public:
			resource_manager_internal_accessor(
				std::function<std::shared_ptr<resource_base>(const std::type_info &, const std::string &, bool)>
				ref_func_by_id,
				std::function<std::shared_ptr<resource_base>(const std::type_info &, const std::string &,
				                                             const std::string &, bool)> ref_func_by_path);
			[[nodiscard]] std::shared_ptr<resource_base> get_resource_by_type_and_id(
				const std::type_info &type, const std::string &id,
				bool is_valid_not_preload_warning) const;
			[[nodiscard]] std::shared_ptr<resource_base> get_resource_by_type_and_path(
				const std::type_info &type, const std::string &path,
				const std::string &root_path,
				bool is_valid_not_preload_warning) const;

		private:
			std::function<std::shared_ptr<resource_base>(const std::type_info &type, const std::string &, bool)>
			ref_func_by_id_;
			std::function<std::shared_ptr<resource_base>(const std::type_info &type, const std::string &,
			                                             const std::string &, bool)> ref_func_by_path_;
		};
	}

	class resource_referencer {
	public:
		resource_referencer(private_::resource_manager_internal_accessor &res_mgr_accessor, std::string root_path,
		                    std::vector<std::shared_ptr<resource_base>> &reference_list);
		/*IDでリソースを参照する。必要なら新たに読み込む*/
		[[nodiscard]] std::shared_ptr<resource_base> reference_resource_by_type_and_id(
			const std::type_info &type, const std::string &id) const;

		template <class T>
		[[nodiscard]] std::shared_ptr<T> reference_resource_by_id(const std::string &id) {
			static_assert(std::is_base_of_v<resource_base, T>, "T must derive ResourceBase");
			if (const auto rsc = reference_resource_by_type_and_id(typeid(T), id)) {
				if (auto out = std::dynamic_pointer_cast<T>(rsc)) { return out; }
				PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(T).name(), "\")");
				return nullptr;
			}
			return nullptr;
		}

		/*Pathでリソースを参照する。必要なら新たに読み込む。パスは現在のリソースパスを基準にした相対パスとなる。*/
		[[nodiscard]] std::shared_ptr<resource_base> reference_resource_by_type_and_path(const std::type_info &type,
			const std::string &path) const;

		template <class T>
		[[nodiscard]] std::shared_ptr<T> reference_resource_by_path(const std::string &path) {
			static_assert(std::is_base_of_v<resource_base, T>, "T must derive ResourceBase");
			if (const auto rsc = reference_resource_by_type_and_path(typeid(T), path)) {
				if (auto out = std::dynamic_pointer_cast<T>(rsc)) { return out; }
				PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(T).name(), "\")");
				return nullptr;
			}
			return nullptr;
		}

		/*ID又はPathでリソースを参照する。必要なら新たに読み込む。重複した場合はIDが優先される。パスは現在のリソースパスを基準にした相対パスとなる。*/
		[[nodiscard]] std::shared_ptr<resource_base> reference_resource_by_type_and_id_or_path(
			const std::type_info &type, const std::string &id_or_path) const;

		template <class T>
		[[nodiscard]] std::shared_ptr<T> reference_resource_by_i_dor_path(const std::string &id_or_path) {
			static_assert(std::is_base_of_v<resource_base, T>, "T must derive ResourceBase");
			if (const auto rsc = reference_resource_by_type_and_id_or_path(typeid(T), id_or_path)) {
				if (auto out = std::dynamic_pointer_cast<T>(rsc)) { return out; }
				PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(T).name(), "\")");
				return nullptr;
			}
			return nullptr;
		}

	private:
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		std::vector<std::shared_ptr<resource_base>> &reference_list_;
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		private_::resource_manager_internal_accessor &manager_accessor_;
		std::string root_path_;
	};
}
