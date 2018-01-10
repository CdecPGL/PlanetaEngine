#include "ClassInfoCaller.hpp"
#include "ClassInfo.hpp"
#include "ReflectionExceptions.hpp"
#include "StringUtility.hpp"
#include "Reflection.hpp"
#include "Reflectable.hpp"

namespace plnt {
	using namespace private_;

	ClassInfoCaller::ClassInfoCaller(const ClassInfo& class_info)noexcept :class_info_(class_info) {}

	void ClassInfoCaller::SetVariable(const std::string& var_id, Reflectable& obj, const boost::any& v) const {
		auto it = class_info_.public_variable_prpperty_info.find(var_id);
		if (it == class_info_.public_variable_prpperty_info.end()) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("クラス\"",obj_tid,"\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
		}
		try {
			it->second.setter(obj, v);
		} catch (boost::bad_any_cast&) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id, "\"の読み込みにおいて型の不一致エラーが発生しました。(変数型:", it->second.t_info.name(), ", 指定型:", v.type().name(), ")"));
		}
	}

	void ClassInfoCaller::GetVariable(const std::string& var_id, Reflectable& obj, boost::any& v) const {
		auto it = class_info_.public_variable_prpperty_info.find(var_id);
		if (it == class_info_.public_variable_prpperty_info.end()) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
		}
		try {
			v = it->second.getter(obj);
		} catch (boost::bad_any_cast&) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id, "\"の書き込みにおいて型の不一致エラーが発生しました。(変数型:", it->second.t_info.name(), ", 指定型:", v.type().name(), ")"));
		}
	}

	void ClassInfoCaller::SetDataFromPtree(const boost::property_tree::ptree& pt, Reflectable& obj) {
		for (auto&& ptp : pt) {
			auto var_id = ptp.first;
			auto it = class_info_.public_variable_prpperty_info.find(var_id);
			if (it == class_info_.public_variable_prpperty_info.end()) {
				std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
				throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"に変数またはプロパティ\"", var_id, "\"は登録されていません。"));
			}
			try {
				it->second.ptree_loeder(obj, ptp.second);
			}
			catch (reflection_error& e) {
				throw reflection_error(util::ConvertAndConnectToString("変数またはプロパティ\"", var_id, "\"の読み込みに失敗しました。:", e.what()));
			}
		}
	}

	//copy_handlerが例外を投げる可能性があるのでnoexceptではない
	std::shared_ptr<Reflectable> ClassInfoCaller::Clone(Reflectable& src_obj) {
		auto obj = class_info_.creator();
		for (auto&& copy_handler : class_info_.copy_handler_list) {
			copy_handler(src_obj, *obj);
		}
		return obj;
	}

	void ClassInfoCaller::CopyFrom(Reflectable& me, const Reflectable& src) {
		throw reflection_error("Copyは未実装です。");
	}

}