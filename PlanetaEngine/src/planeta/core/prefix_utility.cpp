#include "boost/bimap/bimap.hpp"

#include "prefix_utility.hpp"
#include "system_variables.hpp"
#include "string_utility.hpp"
#include "log_utility.hpp"

namespace plnt::private_ {
	using namespace system_variables;

	namespace {
		using pc_map_type = boost::bimaps::bimap<char, object_category>;

		//マップの初期化クラス
		class prefix_category_map_initializer {
		public:
			prefix_category_map_initializer() {
				using vt = pc_map_type::value_type;
				prefix_category_map.insert(vt(prefixes::game_object, object_category::game_object));
				prefix_category_map.insert(vt(prefixes::game_object_component, object_category::game_object_component));
				prefix_category_map.insert(vt(prefixes::game_object_interface, object_category::game_object_interface));
				prefix_category_map.insert(vt(prefixes::task, object_category::task));
				prefix_category_map.insert(vt(prefixes::resource, object_category::resource));
				prefix_category_map.insert(vt(prefixes::event_arg, object_category::event_arg));
				prefix_category_map.insert(vt(prefixes::scene, object_category::scene));
				prefix_category_map.insert(vt(prefixes::standard_interface, object_category::standard_interface));
			}

			pc_map_type prefix_category_map;
		};

		//boost::bimapは宣言時に初期化をできなさそうなのでこうする。
		pc_map_type &prefix_category_map() {
			static prefix_category_map_initializer initializer;
			return initializer.prefix_category_map;
		}

		std::pair<bool, char> check_prefix_format(const std::string &str) {
			if (str.length() >= 2) {
				const char c1{str[1]};
				if (char c0{str[0]}; util::is_upper_case_character(c0) && util::is_upper_case_character(c1)) {
					return {true, c0};
				}
				return {false, static_cast<char>(0)};
			}
			return {false, static_cast<char>(0)};
		}

		object_category detect_category_by_character(const char c) {
			decltype(auto) map = prefix_category_map().left;
			const auto it = map.find(c);
			return it == map.end() ? object_category::others : it->second;
		}
	}

	std::string remove_prefix(const std::string &class_name) {
		if (const auto [is_exist, prefix] = check_prefix_format(class_name); is_exist) {
			if (detect_category_by_character(prefix) != object_category::others) {
				return class_name.substr(1, class_name.size() - 1);
			}
			return class_name;
		}
		return class_name;
	}

	object_category detect_category_by_prefix(const std::string &class_name) {
		if (const auto [is_exist, prefix] = check_prefix_format(class_name); is_exist) {
			return detect_category_by_character(prefix);
		}
		return object_category::others;
	}

	std::string add_prefix(const std::string &class_id, const object_category category) {
		decltype(auto) map = prefix_category_map().right;
		const auto it = map.find(category);
		if (it == map.end()) { return class_id; }
		std::string buf;
		buf.push_back(it->second);
		return buf + class_id;
	}
}
