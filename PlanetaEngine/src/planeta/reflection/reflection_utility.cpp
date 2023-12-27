#include "reflection_utility.hpp"
#include "reflection.hpp"
#include "reflectable.hpp"
#include "reflectable_class_accessor.hpp"

namespace plnt::reflection::private_ {
	void reflective_ptree_converter_from_reflection_system(reflectable &dst, const boost::property_tree::ptree &src) {
		const auto rca = reflection::get_reflectable_class_accessor(typeid(dst));
		rca->load_from_ptree(dst, src);
	}

	void reflective_copy_from_reflection_system(reflectable &dst, const reflectable &src) {
		const auto rca = reflection::get_reflectable_class_accessor(typeid(dst));
		rca->copy(dst, src);
	}
}
