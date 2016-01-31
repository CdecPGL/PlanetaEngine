#include "DataContainer.h"
#include "DataContainerImpl.h"
#include "DataContainerError.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace utility {
		DataContainer::DataContainer() :impl_(std::make_unique<Impl_>()) {}

		DataContainer::DataContainer(const DataContainer& obj) : impl_(std::make_unique<Impl_>(*obj.impl_)) {}

		DataContainer::DataContainer(DataContainer&& obj):impl_(std::move(obj.impl_)) {
			obj.impl_ = std::make_unique<Impl_>();
		}

		DataContainer::~DataContainer() = default;

		DataContainer& DataContainer::operator=(const DataContainer& obj) {
			*impl_ = *obj.impl_;
			return *this;
		}

		DataContainer& DataContainer::operator=(DataContainer&& obj) {
			impl_ = std::move(obj.impl_);
			obj.impl_ = std::make_unique<Impl_>();
			return *this;
		}

		int64_t DataContainer::GetInteger(const std::string& id) const {
			try {
				impl_->CheckElementExist(id);
				impl_->CheckElementType(id, typeid(data_container_element_type::integer));
				return impl_->integer_elements_.at(id);
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
				return 0;
			}
		}

		std::string DataContainer::GetString(const std::string& id) const {
			try {
				impl_->CheckElementExist(id);
				impl_->CheckElementType(id, typeid(data_container_element_type::string));
				return impl_->string_elements_.at(id);
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
				return "NULL";
			}
		}

		bool DataContainer::GetBoolean(const std::string& id) const {
			try {
				impl_->CheckElementExist(id);
				impl_->CheckElementType(id, typeid(data_container_element_type::boolean));
				return impl_->boolean_elements_.at(id);
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
				return false;
			}
		}

		double DataContainer::GetNumber(const std::string& id) const {
			try {
				impl_->CheckElementExist(id);
				impl_->CheckElementType(id, typeid(data_container_element_type::number));
				return impl_->number_elements_.at(id);
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
				return 0.0;
			}
		}

		void DataContainer::SetInteger(const std::string& id, int64_t value) {
			const auto& tid = typeid(data_container_element_type::integer);
			auto& e_map = impl_->integer_elements_;
			try {
				if (impl_->IsExistElement(id)) {
					impl_->CheckElementType(id, tid);
					e_map.at(id) = value;
				} else {
					impl_->RegisterElementType(id, tid);
					e_map.emplace(id, value);
				}
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
			}
		}

		void DataContainer::SetString(const std::string& id, const std::string& value) {
			const auto& tid = typeid(data_container_element_type::string);
			auto& e_map = impl_->string_elements_;
			try {
				if (impl_->IsExistElement(id)) {
					impl_->CheckElementType(id, tid);
					e_map.at(id) = value;
				} else {
					impl_->RegisterElementType(id, tid);
					e_map.emplace(id, value);
				}
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
			}
		}

		void DataContainer::SetBoolean(const std::string& id, bool value) {
			const auto& tid = typeid(data_container_element_type::boolean);
			auto& e_map = impl_->boolean_elements_;
			try {
				if (impl_->IsExistElement(id)) {
					impl_->CheckElementType(id, tid);
					e_map.at(id) = value;
				} else {
					impl_->RegisterElementType(id, tid);
					e_map.emplace(id, value);
				}
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
			}
		}

		void DataContainer::SetNumber(const std::string& id, double value) {
			const auto& tid = typeid(data_container_element_type::number);
			auto& e_map = impl_->number_elements_;
			try {
				if (impl_->IsExistElement(id)) {
					impl_->CheckElementType(id, tid);
					e_map.at(id) = value;
				} else {
					impl_->RegisterElementType(id, tid);
					e_map.emplace(id, value);
				}
			} catch (DataContainerError& e) {
				debug::SystemLog::instance().LogError(e.what(), __FUNCTION__);
			}
		}
	}
}