#pragma  once

#include "Reflection.h"

namespace planeta {
	using namespace core;

	std::shared_ptr<Object> Reflection::CreateObjectByID(const std::string& id) {
		auto it = id_creator_map_.find(id);
		if (it == id_creator_map_.end()) {
			PE_LOG_ERROR("存在しないObject(ID:", id, ")が指定されました。");
			return nullptr;
		} else {
			return (it->second)();
		}
	}

	std::shared_ptr<Object> Reflection::CreateObjectByTypeInfo(const std::type_info& t_info) {
		auto it = type_creator_map_.find(t_info);
		if (it == type_creator_map_.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。");
			return nullptr;
		} else {
			return (it->second)();
		}
	}

	Reflection::Reflection() = default;

}