#include <cassert>

#include "DataContainerImpl.h"
#include "DataContainerError.h"
#include "StringUtility.h"

namespace planeta {
	namespace util {
		DataContainer::Impl_::Impl_(const Impl_& obj) :
			element_type_map_(obj.element_type_map_),
			integer_elements_(obj.integer_elements_),
			boolean_elements_(obj.boolean_elements_),
			string_elements_(obj.string_elements_),
			number_elements_(obj.number_elements_) {}

		DataContainer::Impl_& DataContainer::Impl_::operator=(const Impl_& obj) {
			element_type_map_ = obj.element_type_map_;
			integer_elements_ = obj.integer_elements_;
			boolean_elements_ = obj.boolean_elements_;
			string_elements_ = obj.string_elements_;
			number_elements_ = obj.number_elements_;
			return *this;
		}

		void DataContainer::Impl_::CheckElementType(const std::string& id, const std::type_info& type)const {
			auto it = element_type_map_.find(id);
			assert(it != element_type_map_.end()); //ë∂ç›ÇµÇƒÇ¢ÇÈóvëfÇ≈Ç»ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢ÅB
			if (it->second != type) {
				std::string err(util::ConvertAndConnectToString("å^Ç™àŸÇ»ÇËÇ‹Ç∑ÅB(ID:", id, ",å^:", it->second.name(), ",éwíËå^:", type.name(), ")"));
				throw DataContainerError(err);
			}
		}

		void DataContainer::Impl_::CheckElementExist(const std::string& id)const {
			if (element_type_map_.find(id) == element_type_map_.end()) {
				std::string err(util::ConvertAndConnectToString("óvëfÇ™ë∂ç›ÇµÇ‹ÇπÇÒÅB(ID:", id, ")"));
				throw DataContainerError(err);
			}
		}

		void DataContainer::Impl_::RegisterElementType(const std::string& id, const std::type_info& type) {
			assert(element_type_map_.find(id) == element_type_map_.end()); //ë∂ç›ÇµÇƒÇ¢Ç»Ç¢óvëfÇ≈Ç»ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢
			element_type_map_.emplace(id, type);
		}

		bool DataContainer::Impl_::IsExistElement(const std::string& id) const {
			return !(element_type_map_.find(id) == element_type_map_.end());
		}

		void DataContainer::Impl_::ConstructElementTypeMap() {
			auto elem_register_func = [this](const auto& elem_lst) {
				for (auto&& ep : elem_lst) {
					element_type_map_.emplace(ep.first, typeid(ep.second));
				}
			};
			element_type_map_.clear();
			elem_register_func(integer_elements_);
			elem_register_func(boolean_elements_);
			elem_register_func(string_elements_);
			elem_register_func(number_elements_);
		}

	}
}