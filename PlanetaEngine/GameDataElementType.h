#pragma once

#include <string>
#include <unordered_map>
#include "boost/bimap/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "GameDataComplexType.h"
#include "GameDataError.h"

namespace planeta_engine {
	namespace core {
		class GameDataElementType {
		public:
			enum class Type { int32_type, int64_type, bool_type, double_type, string_type, complex_type,nil };
			static std::string ConvertTypeToTypeID(Type type)noexcept;
			static Type ConvertTypeIDToType(const std::string type_id)noexcept;
			static void ResistComplexType(const std::string& type_id,GameDataComplexType&& complex_type)noexcept;
			static const GameDataComplexType& GetComplexType(const std::string& type_id);
			static bool IsTypeExist(const std::string& type_id) noexcept;
		private:
			static boost::bimaps::bimap<boost::bimaps::unordered_set_of<Type>, boost::bimaps::unordered_set_of<std::string>> type_tid_map_;
			static std::unordered_map<std::string, GameDataComplexType> complex_type_map_;
		};
	}
}
