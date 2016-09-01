#include "ClassInfoCaller.h"
#include "ClassInfo.h"
#include "ReflectionExceptions.h"
#include "StringUtility.h"
#include "Reflection.h"
#include "Reflectable.h"

namespace planeta {
	using namespace private_;

	ClassInfoCaller::ClassInfoCaller(const ClassInfo& class_info)noexcept :class_info_(class_info) {}

	void ClassInfoCaller::SetVariable(const std::string& var_id, Reflectable& obj, const boost::any& v) const {
		auto it = class_info_.public_variable_prpperty_info.find(var_id);
		if (it == class_info_.public_variable_prpperty_info.end()) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("�N���X\"",obj_tid,"\"�ɕϐ��܂��̓v���p�e�B\"", var_id, "\"�͓o�^����Ă��܂���B"));
		}
		try {
			it->second.setter(obj, v);
		} catch (boost::bad_any_cast&) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("�N���X\"", obj_tid, "\"�̕ϐ��܂��̓v���p�e�B\"", var_id, "\"�̓ǂݍ��݂ɂ����Č^�̕s��v�G���[���������܂����B(�ϐ��^:", it->second.t_info.name(), ", �w��^:", v.type().name(), ")"));
		}
	}

	void ClassInfoCaller::GetVariable(const std::string& var_id, Reflectable& obj, boost::any& v) const {
		auto it = class_info_.public_variable_prpperty_info.find(var_id);
		if (it == class_info_.public_variable_prpperty_info.end()) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("�N���X\"", obj_tid, "\"�ɕϐ��܂��̓v���p�e�B\"", var_id, "\"�͓o�^����Ă��܂���B"));
		}
		try {
			v = it->second.getter(obj);
		} catch (boost::bad_any_cast&) {
			std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
			throw reflection_error(util::ConvertAndConnectToString("�N���X\"", obj_tid, "\"�̕ϐ��܂��̓v���p�e�B\"", var_id, "\"�̏������݂ɂ����Č^�̕s��v�G���[���������܂����B(�ϐ��^:", it->second.t_info.name(), ", �w��^:", v.type().name(), ")"));
		}
	}

	void ClassInfoCaller::SetDataFromPtree(const boost::property_tree::ptree& pt, Reflectable& obj) {
		for (auto&& ptp : pt) {
			auto var_id = ptp.first;
			auto it = class_info_.public_variable_prpperty_info.find(var_id);
			if (it == class_info_.public_variable_prpperty_info.end()) {
				std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(obj));
				throw reflection_error(util::ConvertAndConnectToString("�N���X\"", obj_tid, "\"�ɕϐ��܂��̓v���p�e�B\"", var_id, "\"�͓o�^����Ă��܂���B"));
			}
			try {
				it->second.ptree_loeder(obj, ptp.second);
			}
			catch (reflection_error& e) {
				throw reflection_error(util::ConvertAndConnectToString("�ϐ��܂��̓v���p�e�B\"", var_id, "\"�̓ǂݍ��݂Ɏ��s���܂����B:", e.what()));
			}
		}
	}

	//copy_handler����O�𓊂���\��������̂�noexcept�ł͂Ȃ�
	std::shared_ptr<Reflectable> ClassInfoCaller::Clone(Reflectable& src_obj) {
		auto obj = class_info_.creator();
		for (auto&& copy_handler : class_info_.copy_handler_list) {
			copy_handler(src_obj, *obj);
		}
		return obj;
	}

	void ClassInfoCaller::CopyFrom(Reflectable& me, const Reflectable& src) {
		throw reflection_error("Copy�͖������ł��B");
	}

}