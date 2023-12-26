#include "ResourceReferencer.hpp"

namespace plnt {
	ResourceReferencer::ResourceReferencer(private_::ResourceManagerInternalAccessor &res_mgr_acsr,
	                                       const std::string &root_path,
	                                       std::vector<std::shared_ptr<ResourceBase>> &reference_list) :
		manager_accessor_(res_mgr_acsr), reference_list_(reference_list), root_path_(root_path) { }

	std::shared_ptr<ResourceBase> ResourceReferencer::ReferenceResourceByTypeAndID(
		const std::type_info &type, const std::string &id) {
		auto res = manager_accessor_.GetResourceByTypeAndID(type, id, false);
		if (res) {
			reference_list_.push_back(res);
			return res;
		} else { return nullptr; }
	}

	std::shared_ptr<ResourceBase> ResourceReferencer::ReferenceResourceByTypeAndPath(
		const std::type_info &type, const std::string &path) {
		auto res = manager_accessor_.GetResourceByTypeAndPath(type, path, root_path_, false);
		if (res) {
			reference_list_.push_back(res);
			return res;
		} else { return nullptr; }
	}

	std::shared_ptr<ResourceBase> ResourceReferencer::ReferenceResourceByTypeAndIDorPath(
		const std::type_info &type, const std::string &id_or_path) {
		auto res = manager_accessor_.GetResourceByTypeAndID(type, id_or_path, false);
		if (res) {
			reference_list_.push_back(res);
			return res;
		} else {
			res = manager_accessor_.GetResourceByTypeAndPath(type, id_or_path, root_path_, false);
			if (res) {
				reference_list_.push_back(res);
				return res;
			} else { return nullptr; }
		}
	}

	namespace private_ {
		ResourceManagerInternalAccessor::ResourceManagerInternalAccessor(
			const std::function<std::shared_ptr<ResourceBase>(const std::type_info &, const std::string &, bool)> &
			ref_func_by_id,
			const std::function<std::shared_ptr<ResourceBase>(const std::type_info &, const std::string &,
			                                                  const std::string &, bool)> &ref_func_by_path) :
			ref_func_by_id_(ref_func_by_id), ref_func_by_path_(ref_func_by_path) { }

		std::shared_ptr<plnt::ResourceBase> ResourceManagerInternalAccessor::GetResourceByTypeAndID(
			const std::type_info &type, const std::string &id, bool is_valid_not_preload_warning) {
			return ref_func_by_id_(type, id, is_valid_not_preload_warning);
		}

		std::shared_ptr<plnt::ResourceBase> ResourceManagerInternalAccessor::GetResourceByTypeAndPath(
			const std::type_info &type, const std::string &path, const std::string &root_path,
			bool is_valid_not_preload_warning) {
			return ref_func_by_path_(type, path, root_path, is_valid_not_preload_warning);
		}
	}
}
