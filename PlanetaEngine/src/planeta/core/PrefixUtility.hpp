#pragma once

#include <string>

namespace plnt::private_ {
	/*接頭辞で用いるオブジェクトのカテゴリ*/
	enum class object_category {
		game_object,
		game_object_component,
		game_object_interface,
		task,
		resource,
		event_arg,
		scene,
		standard_interface,
		others
	};

	/*接頭辞があったら除去する*/
	std::string remove_prefix(const std::string &class_name);
	/*接頭辞によりオブジェクトのカテゴリを判断する*/
	object_category detect_category_by_prefix(const std::string &class_name);
	/*接頭辞を追加する*/
	std::string add_prefix(const std::string &class_id, object_category category);
}
