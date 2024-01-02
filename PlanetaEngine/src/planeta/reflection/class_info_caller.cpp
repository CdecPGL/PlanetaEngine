#include "..\core\string_utility.hpp"

#include "class_info_caller.hpp"
#include "class_info.hpp"
#include "reflection_exceptions.hpp"
#include "reflection.hpp"
#include "reflectable.hpp"

namespace plnt::reflection {
	using namespace private_;
	using namespace util;

	class_info_caller::class_info_caller(const class_info &class_info) noexcept : class_info_(class_info) { }

	void class_info_caller::set_variable(const std::string &var_id, reflectable &obj, const boost::any &v) const {
		const auto it = class_info_.public_variable_property_info.find(var_id);
		if (it == class_info_.public_variable_property_info.end()) {
			std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(obj));
			throw reflection_error(
				convert_and_connect_to_string("クラス\"", obj_tid, "\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
		}
		try { it->second.setter(obj, v); } catch (boost::bad_any_cast &) {
			std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(obj));
			throw reflection_error(convert_and_connect_to_string("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id,
			                                                 "\"の読み込みにおいて型の不一致エラーが発生しました。(変数型:",
			                                                 it->second.t_info.name(), ", 指定型:", v.type().name(), ")"));
		}
	}

	void class_info_caller::get_variable(const std::string &var_id, reflectable &obj, boost::any &v) const {
		const auto it = class_info_.public_variable_property_info.find(var_id);
		if (it == class_info_.public_variable_property_info.end()) {
			std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(obj));
			throw reflection_error(
				convert_and_connect_to_string("クラス\"", obj_tid, "\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
		}
		try { v = it->second.getter(obj); } catch (boost::bad_any_cast &) {
			std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(obj));
			throw reflection_error(convert_and_connect_to_string("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id,
			                                                 "\"の書き込みにおいて型の不一致エラーが発生しました。(変数型:",
			                                                 it->second.t_info.name(), ", 指定型:", v.type().name(), ")"));
		}
	}

	void class_info_caller::set_data_from_ptree(const boost::property_tree::ptree &pt, reflectable &obj) const {
		for (auto &&ptp : pt) {
			auto var_id = ptp.first;
			auto it = class_info_.public_variable_property_info.find(var_id);
			if (it == class_info_.public_variable_property_info.end()) {
				std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(obj));
				throw reflection_error(
					convert_and_connect_to_string("クラス\"", obj_tid, "\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
			}
			try { it->second.ptree_loader(obj, ptp.second); } catch (reflection_error &e) {
				throw reflection_error(convert_and_connect_to_string("変数またはプロパティ\"", var_id, "\"の読み込みに失敗しました。:", e.what()));
			}
		}
	}

	//copy_handlerが例外を投げる可能性があるのでnoexceptではない
	std::shared_ptr<reflectable> class_info_caller::clone(const reflectable &src_obj) const {
		auto obj = class_info_.creator();
		for (auto &&copy_handler : class_info_.copy_handler_list) { copy_handler(src_obj, *obj); }
		return obj;
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	auto class_info_caller::copy_from(reflectable &me, const reflectable &src) -> void { throw reflection_error("Copyは未実装です。"); }
}
