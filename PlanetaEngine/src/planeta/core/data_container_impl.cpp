#include <cassert>

#include "data_container_impl.hpp"
#include "data_container_error.hpp"
#include "StringUtility.hpp"

namespace plnt::util {
	void data_container::impl::check_element_type(const std::string &id, const std::type_info &type) const {
		const auto it = element_type_map_.find(id);
		assert(it != element_type_map_.end()); //存在している要素でなければならない。
		if (it->second != type) {
			const std::string err(ConvertAndConnectToString("型が異なります。(ID:", id, ",型:", it->second.name(), ",指定型:",
			                                                type.name(), ")"));
			throw data_container_error(err);
		}
	}

	void data_container::impl::check_element_exist(const std::string &id) const {
		if (!element_type_map_.contains(id)) {
			const std::string err(ConvertAndConnectToString("要素が存在しません。(ID:", id, ")"));
			throw data_container_error(err);
		}
	}

	void data_container::impl::register_element_type(const std::string &id, const std::type_info &type) {
		assert(element_type_map_.find(id) == element_type_map_.end()); //存在していない要素でなければならない
		element_type_map_.emplace(id, type);
	}

	bool data_container::impl::is_exist_element(const std::string &id) const {
		return !(!element_type_map_.contains(id));
	}

	void data_container::impl::construct_element_type_map() {
		auto elem_register_func = [this](const auto &elem_lst) {
			for (auto &&ep : elem_lst) { element_type_map_.emplace(ep.first, typeid(ep.second)); }
		};
		element_type_map_.clear();
		elem_register_func(integer_elements);
		elem_register_func(boolean_elements);
		elem_register_func(string_elements);
		elem_register_func(number_elements);
	}
}
