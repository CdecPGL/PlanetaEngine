#include"GameDataComplexType.h"
#include "GameDataElementType.h"
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {

		bool GameDataComplexType::IsElementExist(const std::string& element_id) const noexcept
		{
			return elements_.find(element_id) != elements_.end();
		}

		bool GameDataComplexType::CheckElementType(const std::string& element_id, const GameDataElement& element) const noexcept
		{
			auto my_tid = GetElementType(element_id);
			if (my_tid == GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil)) { return false; }
			return my_tid == element.type_id();
		}

		const std::string GameDataComplexType::GetElementType(const std::string& element_id) const noexcept
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				return GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil);
			}
			else {
				return it->second;
			}
		}

		void GameDataComplexType::AddElement(const std::string& element_id, const std::string& element_type_id)
		{
			if (!GameDataElementType::IsTypeExist(element_type_id)) {
				throw GameDataError(std::string("ë∂ç›ÇµÇ»Ç¢GameDataå^" + element_type_id + "ÇGameDataï°çáå^Ç…í«â¡ÇµÇÊÇ§Ç∆ÇµÇ‹ÇµÇΩÅB"));
			}
			else {
				elements_.emplace(element_id, element_type_id);
			}
		}

	}
}