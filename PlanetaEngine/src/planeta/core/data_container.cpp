#include "data_container.hpp"
#include "data_container_impl.hpp"
#include "data_container_error.hpp"
#include "LogUtility.hpp"

namespace plnt::util {
	data_container::data_container() : impl_(std::make_unique<impl>()) { }

	data_container::data_container(data_container &&obj) noexcept: impl_(std::move(obj.impl_)) {
		obj.impl_ = std::make_unique<impl>();
	}

	data_container::~data_container() = default;

	data_container &data_container::operator=(data_container &&obj) noexcept {
		impl_ = std::move(obj.impl_);
		obj.impl_ = std::make_unique<impl>();
		return *this;
	}

	int64_t data_container::get_integer(const std::string &id) const {
		try {
			impl_->check_element_exist(id);
			impl_->check_element_type(id, typeid(data_container_element_type::integer));
			return impl_->integer_elements.at(id);
		} catch (data_container_error &e) {
			PE_LOG_ERROR(e.what());
			return 0;
		}
	}

	std::string data_container::get_string(const std::string &id) const {
		try {
			impl_->check_element_exist(id);
			impl_->check_element_type(id, typeid(data_container_element_type::string));
			return impl_->string_elements.at(id);
		} catch (data_container_error &e) {
			PE_LOG_ERROR(e.what());
			return "NULL";
		}
	}

	bool data_container::get_boolean(const std::string &id) const {
		try {
			impl_->check_element_exist(id);
			impl_->check_element_type(id, typeid(data_container_element_type::boolean));
			return impl_->boolean_elements.at(id);
		} catch (data_container_error &e) {
			PE_LOG_ERROR(e.what());
			return false;
		}
	}

	double data_container::get_number(const std::string &id) const {
		try {
			impl_->check_element_exist(id);
			impl_->check_element_type(id, typeid(data_container_element_type::number));
			return impl_->number_elements.at(id);
		} catch (data_container_error &e) {
			PE_LOG_ERROR(e.what());
			return 0.0;
		}
	}

	void data_container::set_integer(const std::string &id, int64_t value) const {
		const auto &tid = typeid(data_container_element_type::integer);
		auto &e_map = impl_->integer_elements;
		try {
			if (impl_->is_exist_element(id)) {
				impl_->check_element_type(id, tid);
				e_map.at(id) = value;
			} else {
				impl_->register_element_type(id, tid);
				e_map.emplace(id, value);
			}
		} catch (data_container_error &e) { PE_LOG_ERROR(e.what()); }
	}

	void data_container::set_string(const std::string &id, const std::string &value) const {
		const auto &tid = typeid(data_container_element_type::string);
		auto &e_map = impl_->string_elements;
		try {
			if (impl_->is_exist_element(id)) {
				impl_->check_element_type(id, tid);
				e_map.at(id) = value;
			} else {
				impl_->register_element_type(id, tid);
				e_map.emplace(id, value);
			}
		} catch (data_container_error &e) { PE_LOG_ERROR(e.what()); }
	}

	void data_container::set_boolean(const std::string &id, bool value) const {
		const auto &tid = typeid(data_container_element_type::boolean);
		auto &e_map = impl_->boolean_elements;
		try {
			if (impl_->is_exist_element(id)) {
				impl_->check_element_type(id, tid);
				e_map.at(id) = value;
			} else {
				impl_->register_element_type(id, tid);
				e_map.emplace(id, value);
			}
		} catch (data_container_error &e) { PE_LOG_ERROR(e.what()); }
	}

	void data_container::set_number(const std::string &id, double value) const {
		const auto &tid = typeid(data_container_element_type::number);
		auto &e_map = impl_->number_elements;
		try {
			if (impl_->is_exist_element(id)) {
				impl_->check_element_type(id, tid);
				e_map.at(id) = value;
			} else {
				impl_->register_element_type(id, tid);
				e_map.emplace(id, value);
			}
		} catch (data_container_error &e) { PE_LOG_ERROR(e.what()); }
	}
}
