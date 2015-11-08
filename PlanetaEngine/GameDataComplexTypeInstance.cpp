#include "GameDataComplexTypeInstance.h"
#include "GameDataElementType.h"
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {

		const GameDataElement& GameDataComplexTypeInstance::GetElement(const std::string& element_id) const
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				std::string element_type_id = complex_type().GetElementType(element_id);
				if (complex_type().IsElementExist(element_id)) {
					return elements_.emplace(element_id, std::move(GameDataElement(element_type_id))).first->second;
				}
				else {
					throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "に存在しない要素" + element_id + "にアクセスしようとしました。");
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
				if (!complex_type().IsElementExist(element_id)) { throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "に存在しない要素" + element_id + "にアクセスしようとしました。"); }
				if (!complex_type().CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "の要素" + element_id + "(" + complex_type().GetElementType(element_id) + "型)に" + element.type_id() + "の要素を格納しようとしましたが、型が一致していません。");
				}
				elements_.emplace(element_id, element);
			}
			else {
				if (!complex_type().CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "の要素" + element_id + "(" + complex_type().GetElementType(element_id) + "型)に" + element.type_id() + "の要素を格納しようとしましたが、型が一致していません。");
				}
				it->second = element;
			}
		}

		void GameDataComplexTypeInstance::SetElement(const std::string& element_id, GameDataElement&& element)
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				if (!complex_type().IsElementExist(element_id)) { throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "に存在しない要素" + element_id + "にアクセスしようとしました。"); }
				if (!complex_type().CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "の要素" + element_id + "(" + complex_type().GetElementType(element_id) + "型)に" + element.type_id() + "の要素を格納しようとしましたが、型が一致していません。");
				}
				elements_.emplace(element_id, std::move(element));
			}
			else {
				if (!complex_type().CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData複合型") + complex_type().type_id() + "の要素" + element_id + "(" + complex_type().GetElementType(element_id) + "型)に" + element.type_id() + "の要素を格納しようとしましたが、型が一致していません。");
				}
				it->second = std::move(element);
			}
		}

	}
}