#include"GameDataComplexType.h"
#include "GameDataElementType.h"
#include "GameDataElement.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {

		GameDataComplexType::GameDataComplexType(const std::string type_id, std::unordered_map<std::string, std::string>&& element_types):type_name_(type_id), element_types_(std::move(element_types))
		{
		}

		bool GameDataComplexType::IsElementExist(const std::string& element_id) const noexcept
		{
			return element_types_.find(element_id) != element_types_.end();
		}

		bool GameDataComplexType::CheckElementType(const std::string& element_id, const GameDataElement& element) const noexcept
		{
			auto my_tid = GetElementType(element_id);
			if (my_tid == GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil)) { return false; }
			return my_tid == element.type_id();
		}

		const std::string GameDataComplexType::GetElementType(const std::string& element_id) const noexcept
		{
			auto it = element_types_.find(element_id);
			if (it == element_types_.end()) {
				return GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil);
			}
			else {
				return it->second;
			}
		}

		bool GameDataComplexType::TypeCheck()noexcept
		{
			//型チェック
			for (auto& type : element_types_) {
				if (!GameDataElementType::IsTypeExist(type.second)) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないGameData型", type.second, "がGameData複合型", type_name_, "に含まれています。");
					return false;
				}
			}
			return true;
		}
	}
}