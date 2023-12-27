#pragma once

#include <string>
#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

namespace plnt::reflection {
	class reflectable;

	namespace private_ {
		struct class_info;

		class class_info_caller {
		public:
			explicit class_info_caller(const class_info &class_info) noexcept;
			void set_variable(const std::string &var_id, reflectable &obj, const boost::any &v) const;
			void get_variable(const std::string &var_id, reflectable &obj, boost::any &v) const;
			void set_data_from_ptree(const boost::property_tree::ptree &pt, reflectable &obj) const;
			[[nodiscard]] std::shared_ptr<reflectable> clone(const reflectable &src_obj) const;
			void copy_from(reflectable &me, const reflectable &src);

		private:
			const class_info &class_info_;
		};
	}
}
