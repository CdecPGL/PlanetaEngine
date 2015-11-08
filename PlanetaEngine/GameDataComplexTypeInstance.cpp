#include "GameDataComplexTypeInstance.h"
#include "GameDataElementType.h"
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {

		const GameDataElement& GameDataComplexTypeInstance::GetElement(const std::string& element_id) const
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				std::string element_type_id = complex_type_.GetElementType(element_id);
				if (complex_type_.IsElementExist(element_id)) {
					return elements_.emplace(element_id, std::move(GameDataElement(element_type_id))).first->second;
				}
				else {
					throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚É‘¶Ý‚µ‚È‚¢—v‘f" + element_id + "‚ÉƒAƒNƒZƒX‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B");
				}
			}
			else {
				return it->second;
			}
		}

		void GameDataComplexTypeInstance::SetElement(const std::string& element_id, const GameDataElement& element)
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				if (!complex_type_.IsElementExist(element_id)) { throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚É‘¶Ý‚µ‚È‚¢—v‘f" + element_id + "‚ÉƒAƒNƒZƒX‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B"); }
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚Ì—v‘f" + element_id + "(" + complex_type_.GetElementType(element_id) + "Œ^)‚É" + element.type_id() + "‚Ì—v‘f‚ðŠi”[‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½‚ªAŒ^‚ªˆê’v‚µ‚Ä‚¢‚Ü‚¹‚ñB");
				}
				elements_.emplace(element_id, element);
			}
			else {
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚Ì—v‘f" + element_id + "(" + complex_type_.GetElementType(element_id) + "Œ^)‚É" + element.type_id() + "‚Ì—v‘f‚ðŠi”[‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½‚ªAŒ^‚ªˆê’v‚µ‚Ä‚¢‚Ü‚¹‚ñB");
				}
				it->second = element;
			}
		}

		void GameDataComplexTypeInstance::SetElement(const std::string& element_id, GameDataElement&& element)
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				if (!complex_type_.IsElementExist(element_id)) { throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚É‘¶Ý‚µ‚È‚¢—v‘f" + element_id + "‚ÉƒAƒNƒZƒX‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B"); }
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚Ì—v‘f" + element_id + "(" + complex_type_.GetElementType(element_id) + "Œ^)‚É" + element.type_id() + "‚Ì—v‘f‚ðŠi”[‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½‚ªAŒ^‚ªˆê’v‚µ‚Ä‚¢‚Ü‚¹‚ñB");
				}
				elements_.emplace(element_id, std::move(element));
			}
			else {
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData•¡‡Œ^") + complex_type_.type_id() + "‚Ì—v‘f" + element_id + "(" + complex_type_.GetElementType(element_id) + "Œ^)‚É" + element.type_id() + "‚Ì—v‘f‚ðŠi”[‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½‚ªAŒ^‚ªˆê’v‚µ‚Ä‚¢‚Ü‚¹‚ñB");
				}
				it->second = std::move(element);
			}
		}

	}
}