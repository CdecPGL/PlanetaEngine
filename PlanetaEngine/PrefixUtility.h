#pragma once

#include <string>

namespace planeta {
	namespace core {
		enum class ObjectCategory {
			GameObject,
			GameObjectComponent,
			GameObjectInterface,
			Task,
			Resource,
			EventArg,
			Scene,
			StandardInterface,
			Others
		};
		std::string RemovePrefix(const std::string& class_name);
		ObjectCategory DetectCategoryByPrefix(const std::string& class_name);
		std::string AddPrefix(const std::string& class_id,ObjectCategory category);
	}
}