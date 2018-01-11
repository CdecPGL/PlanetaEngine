#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <unordered_set>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

#include "TypeInfoWrapper.hpp"

struct lua_State;
namespace plnt {
	class Reflectable;
	namespace private_ {
		struct VariableOrPropertyInfo {
			VariableOrPropertyInfo(const std::type_info& ti) :t_info(ti) {}
			const std::type_info& t_info;
			bool is_writable = false;
			bool is_readable = false;
			std::function<void(Reflectable&, const boost::any&)> setter;
			std::function<boost::any(Reflectable&)> getter;
			std::function<void(Reflectable&, const boost::property_tree::ptree&)> ptree_loeder;
		};
		struct ClassInfo {
			TypeInfoWrapper this_t_info;
			TypeInfoWrapper super_t_info;
			std::unordered_set<TypeInfoWrapper, TypeInfoWrapper::hash> child_t_info;
			std::vector<std::function<void(const Reflectable&, Reflectable&)>> copy_handler_list;
			const ClassInfo* super_class_info;
			std::unordered_set<const ClassInfo*> child_class_info;
			std::string object_type_id;
			bool is_bind_to_script = true;
			bool is_abstract;
			std::function<std::shared_ptr<Reflectable>()> creator;
			std::unordered_map<std::string, VariableOrPropertyInfo> public_variable_prpperty_info;
			std::function<void(lua_State*)> lua_binder;
		};
	}
}