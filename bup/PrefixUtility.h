#pragma once

#include <string>

namespace planeta {
	namespace core {
		/*接頭辞で用いるオブジェクトのカテゴリ*/
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
		/*接頭辞があったら除去する*/
		std::string RemovePrefix(const std::string& class_name);
		/*接頭辞によりオブジェクトのカテゴリを判断する*/
		ObjectCategory DetectCategoryByPrefix(const std::string& class_name);
		/*接頭辞を追加する*/
		std::string AddPrefix(const std::string& class_id,ObjectCategory category);
	}
}