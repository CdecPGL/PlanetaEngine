#pragma once

#include <unordered_map>
#include <typeindex>

#include "boost/serialization/unordered_map.hpp"

#include "data_container.hpp"

namespace plnt::util {
	namespace data_container_element_type {
		using integer = int64_t;
		using boolean = bool;
		using string = std::string;
		using number = double;
	}

	class data_container::impl {
	public:
		impl() = default;
		impl(const impl &obj) = default;
		impl(impl &&obj) = default;
		~impl() = default;
		impl &operator=(const impl &obj) = default;
		impl &operator=(impl &&obj) = default;

		std::unordered_map<std::string, data_container_element_type::integer> integer_elements;
		std::unordered_map<std::string, data_container_element_type::boolean> boolean_elements;
		std::unordered_map<std::string, data_container_element_type::string> string_elements;
		std::unordered_map<std::string, data_container_element_type::number> number_elements;

		void check_element_type(const std::string &id, const std::type_info &type) const;
		void check_element_exist(const std::string &id) const;
		void register_element_type(const std::string &id, const std::type_info &type);
		[[nodiscard]] bool is_exist_element(const std::string &id) const;
		void construct_element_type_map();

	private:
		std::unordered_map<std::string, std::type_index> element_type_map_;
	};
}
