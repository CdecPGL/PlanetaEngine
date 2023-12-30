#include "resource_referencer.hpp"

namespace plnt {
	resource_referencer::resource_referencer(private_::resource_manager_internal_accessor &res_mgr_accessor,
	                                       std::string root_path,
	                                       std::vector<std::shared_ptr<resource_base>> &reference_list) :
		reference_list_(reference_list), manager_accessor_(res_mgr_accessor), root_path_(std::move(root_path)) { }

	std::shared_ptr<resource_base> resource_referencer::reference_resource_by_type_and_id(
		const std::type_info &type, const std::string &id) const {
		if (auto res = manager_accessor_.get_resource_by_type_and_id(type, id, false)) {
			reference_list_.push_back(res);
			return res;
		}
		return nullptr;
	}

	std::shared_ptr<resource_base> resource_referencer::reference_resource_by_type_and_path(
		const std::type_info &type, const std::string &path) const {
		if (auto res = manager_accessor_.get_resource_by_type_and_path(type, path, root_path_, false)) {
			reference_list_.push_back(res);
			return res;
		}
		return nullptr;
	}

	std::shared_ptr<resource_base> resource_referencer::reference_resource_by_type_and_id_or_path(
		const std::type_info &type, const std::string &id_or_path) const {
		if (auto res = manager_accessor_.get_resource_by_type_and_id(type, id_or_path, false)) {
			reference_list_.push_back(res);
			return res;
		} else {
			res = manager_accessor_.get_resource_by_type_and_path(type, id_or_path, root_path_, false);
			if (res) {
				reference_list_.push_back(res);
				return res;
			}
			return nullptr;
		}
	}

	namespace private_ {
		resource_manager_internal_accessor::resource_manager_internal_accessor(
			std::function<std::shared_ptr<resource_base>(const std::type_info &, const std::string &, bool)>
			ref_func_by_id,
			std::function<std::shared_ptr<resource_base>(const std::type_info &, const std::string &,
			                                             const std::string &, bool)> ref_func_by_path) :
			ref_func_by_id_(std::move(ref_func_by_id)), ref_func_by_path_(std::move(ref_func_by_path)) { }

		std::shared_ptr<resource_base> resource_manager_internal_accessor::get_resource_by_type_and_id(
			const std::type_info &type, const std::string &id, const bool is_valid_not_preload_warning) const {
			return ref_func_by_id_(type, id, is_valid_not_preload_warning);
		}

		std::shared_ptr<resource_base> resource_manager_internal_accessor::get_resource_by_type_and_path(
			const std::type_info &type, const std::string &path, const std::string &root_path,
			const bool is_valid_not_preload_warning) const {
			return ref_func_by_path_(type, path, root_path, is_valid_not_preload_warning);
		}
	}
}
