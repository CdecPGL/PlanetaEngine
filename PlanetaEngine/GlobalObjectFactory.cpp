#pragma  once

#include "GlobalObjectFactory.h"

namespace planeta_engine {
	namespace core {
		GlobalObjectFactory& GlobalObjectFactory::instance() {
			static GlobalObjectFactory gof;
			return gof;
		}

		std::shared_ptr<Object> GlobalObjectFactory::CreateObjectByID(const std::string& id) {
			auto it = id_creator_map_.find(id);
			if (it == id_creator_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないObject(ID:", id, "が指定されました。");
				return nullptr;
			} else {
				return (it->second)();
			}
		}

		std::shared_ptr<Object> GlobalObjectFactory::CreateObjectByTypeInfo(const std::type_info& t_info) {
			auto it = type_creator_map_.find(t_info);
			if (it == type_creator_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないObject(Type:", t_info.name(), "が指定されました。");
				return nullptr;
			} else {
				return (it->second)();
			}
		}

		GlobalObjectFactory::GlobalObjectFactory() = default;

	}
}