#pragma once

#include <unordered_map>

namespace planeta_engine{
	namespace core{
		namespace ObjectUtility{
			template<typename Key,class C>
			using unordered_map = std::unordered_map < Key, C > ;
		}
	}
}