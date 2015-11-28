#pragma once

#include <unordered_map>
#include <string>

namespace planeta_engine {
	namespace core {
		class GameDataElement;
		class GameDataComplexType {
		public:
			GameDataComplexType(const std::string type_id, std::unordered_map<std::string, std::string>&& element_types);
			/*情報取得用関数群*/
			const std::string& type_id()const noexcept { return type_name_; }
			bool IsElementExist(const std::string& element_id)const noexcept;
			bool CheckElementType(const std::string& element_id, const GameDataElement& element)const noexcept;
			const std::string GetElementType(const std::string& element_id)const noexcept;
			
			bool TypeCheck()noexcept;
		private:
			std::string type_name_;
			std::unordered_map<std::string, std::string> element_types_; //要素(ID、型)
		};
	}
}