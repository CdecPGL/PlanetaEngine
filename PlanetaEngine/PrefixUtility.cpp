#include "boost/bimap/bimap.hpp"
#include "PrefixUtility.h"
#include "SystemVariables.h"
#include "StringUtility.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		using namespace system_variables;
		namespace {
			using PCMapType = boost::bimaps::bimap<char, ObjectCategory>;
			//�}�b�v�̏������N���X
			class PrefixCategoryMapInitializer {
			public:
				PrefixCategoryMapInitializer(PCMapType& map) {
					using vt = PCMapType::value_type;
					map.insert(vt(prefixes::GameObject, ObjectCategory::GameObject));
					map.insert(vt(prefixes::GameObjectComponent, ObjectCategory::GameObjectComponent));
					map.insert(vt(prefixes::GameObjectInterface, ObjectCategory::GameObjectInterface));
					map.insert(vt(prefixes::Task, ObjectCategory::Task));
					map.insert(vt(prefixes::Resource, ObjectCategory::Resource));
					map.insert(vt(prefixes::EventArg, ObjectCategory::EventArg));
					map.insert(vt(prefixes::Scene, ObjectCategory::Scene));
					map.insert(vt(prefixes::StandardInterface, ObjectCategory::StandardInterface));
				}
			};
			//boost::bimap�͐錾���ɏ��������ł��Ȃ������Ȃ̂ł�������B
			PCMapType& prefix_category_map() {
				static PCMapType prefix_category_map_;
				static PrefixCategoryMapInitializer initializer(prefix_category_map_);
				return prefix_category_map_;
			}

			std::pair<bool, char> CheckPrefixFormat(const std::string& str) {
				if (str.length() >= 2) {
					char c0{ str[0] }, c1{ str[1] };
					if (util::IsUpperCaseCharacter(c0) && util::IsUpperCaseCharacter(c1)) {
						return{ true,c0 };
					} else {
						return{ false,0 };
					}
				} else {
					return{ false,0 };
				}
			}
			
			ObjectCategory DetectCategoryByCharacter(char c) {
				decltype(auto) map = prefix_category_map().left;
				auto it = map.find(c);
				return it == map.end() ? ObjectCategory::Others : it->second;
			}
		}

		std::string RemovePrefix(const std::string& class_name) {
			auto ret = CheckPrefixFormat(class_name);
			if (ret.first) {
				char p = ret.second;
				if (DetectCategoryByCharacter(p) != ObjectCategory::Others) {
					return class_name.substr(1, class_name.size() - 1);
				} else {
					return class_name;
				}
			} else {
				return class_name;
			}
		}

		ObjectCategory DetectCategoryByPrefix(const std::string& class_name) {
			auto ret = CheckPrefixFormat(class_name);
			if (ret.first) {
				char p = ret.second;
				return DetectCategoryByCharacter(p);
			} else {
				return ObjectCategory::Others;
			}
		}

		std::string AddPrefix(const std::string& class_id, ObjectCategory category) {
			decltype(auto) map = prefix_category_map().right;
			auto it = map.find(category);
			if (it == map.end()) {
				return class_id;
			}
			std::string buf;
			buf.push_back(it->second);
			return buf + class_id;
		}

	}
}