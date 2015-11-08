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
					throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�ɑ��݂��Ȃ��v�f" + element_id + "�ɃA�N�Z�X���悤�Ƃ��܂����B");
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
				if (!complex_type_.IsElementExist(element_id)) { throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�ɑ��݂��Ȃ��v�f" + element_id + "�ɃA�N�Z�X���悤�Ƃ��܂����B"); }
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�̗v�f" + element_id + "(" + complex_type_.GetElementType(element_id) + "�^)��" + element.type_id() + "�̗v�f���i�[���悤�Ƃ��܂������A�^����v���Ă��܂���B");
				}
				elements_.emplace(element_id, element);
			}
			else {
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�̗v�f" + element_id + "(" + complex_type_.GetElementType(element_id) + "�^)��" + element.type_id() + "�̗v�f���i�[���悤�Ƃ��܂������A�^����v���Ă��܂���B");
				}
				it->second = element;
			}
		}

		void GameDataComplexTypeInstance::SetElement(const std::string& element_id, GameDataElement&& element)
		{
			auto it = elements_.find(element_id);
			if (it == elements_.end()) {
				if (!complex_type_.IsElementExist(element_id)) { throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�ɑ��݂��Ȃ��v�f" + element_id + "�ɃA�N�Z�X���悤�Ƃ��܂����B"); }
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�̗v�f" + element_id + "(" + complex_type_.GetElementType(element_id) + "�^)��" + element.type_id() + "�̗v�f���i�[���悤�Ƃ��܂������A�^����v���Ă��܂���B");
				}
				elements_.emplace(element_id, std::move(element));
			}
			else {
				if (!complex_type_.CheckElementType(element_id, element)) {
					throw GameDataError(std::string("GameData�����^") + complex_type_.type_id() + "�̗v�f" + element_id + "(" + complex_type_.GetElementType(element_id) + "�^)��" + element.type_id() + "�̗v�f���i�[���悤�Ƃ��܂������A�^����v���Ă��܂���B");
				}
				it->second = std::move(element);
			}
		}

	}
}