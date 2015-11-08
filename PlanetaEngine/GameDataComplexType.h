#pragma once

#include <unordered_map>
#include <string>

namespace planeta_engine {
	namespace core {
		class GameDataElement;
		class GameDataComplexType {
		public:
			/*���擾�p�֐��Q*/
			const std::string& type_id()const noexcept { return type_name_; }
			bool IsElementExist(const std::string& element_id)const noexcept;
			bool CheckElementType(const std::string& element_id, const GameDataElement& element)const noexcept;
			const std::string GetElementType(const std::string& element_id)const noexcept;
			/*���ݒ�p�֐��Q*/
			void AddElement(const std::string& element_id,const std::string& element_type_id);
		private:
			std::string type_name_;
			std::unordered_map<std::string, std::string> elements_; //�v�f(ID�A�^)
		};
	}
}