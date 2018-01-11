#pragma once
#include <unordered_map>
#include <typeindex>
#include "planeta/core/DataContainer.hpp"

namespace plnt {
	namespace util {
		namespace data_container_element_type {
			using integer = int64_t;
			using boolean = bool;
			using string = std::string;
			using number = double;
		}
		class DataContainer::Impl_ {
		public:
			Impl_() = default;
			Impl_(const Impl_& obj);
			Impl_& operator=(const Impl_& obj);

			std::unordered_map<std::string, data_container_element_type::integer> integer_elements_;
			std::unordered_map<std::string, data_container_element_type::boolean> boolean_elements_;
			std::unordered_map<std::string, data_container_element_type::string> string_elements_;
			std::unordered_map<std::string, data_container_element_type::number> number_elements_;

			void CheckElementType(const std::string& id, const std::type_info& type)const;
			void CheckElementExist(const std::string& id)const;
			void RegisterElementType(const std::string& id, const std::type_info& type);
			bool IsExistElement(const std::string& id)const;
			void ConstructElementTypeMap();
		private:
			std::unordered_map<std::string, std::type_index> element_type_map_;
		};
	}
}
