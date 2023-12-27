#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <unordered_set>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

#include "type_info_wrapper.hpp"

// ReSharper disable once CppInconsistentNaming
struct lua_State;

namespace plnt::reflection {
	class reflectable;

	namespace private_ {
		struct variable_or_property_info {
			explicit variable_or_property_info(const std::type_info &ti) : t_info(ti) { }

			const std::type_info &t_info;
			bool is_writable = false;
			bool is_readable = false;
			std::function<void(reflectable &, const boost::any &)> setter;
			std::function<boost::any(reflectable &)> getter;
			std::function<void(reflectable &, const boost::property_tree::ptree &)> ptree_loader;
		};

		struct class_info {
			type_info_wrapper this_t_info;
			type_info_wrapper super_t_info;
			std::unordered_set<type_info_wrapper, type_info_wrapper::hash> child_t_info;
			std::vector<std::function<void(const reflectable &, reflectable &)>> copy_handler_list;
			const class_info *super_class_info;
			std::unordered_set<const class_info *> child_class_info;
			std::string object_type_id;
			bool is_bind_to_script = true;
			bool is_abstract;
			std::function<std::shared_ptr<reflectable>()> creator;
			std::unordered_map<std::string, variable_or_property_info> public_variable_property_info;
			std::function<void(lua_State *)> lua_binder;
		};
	}
}
