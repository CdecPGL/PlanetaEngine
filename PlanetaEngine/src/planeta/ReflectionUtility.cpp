#include "ReflectionUtility.hpp"
#include "Reflection.hpp"
#include "Reflectable.hpp"
#include "ReflectableClassAccessor.hpp"

namespace plnt {
	namespace private_{
		void ReflectivePtreeConverterFromReflectionSystem(Reflectable& dst, const boost::property_tree::ptree& src) {
			auto rca = Reflection::GetRefrectableClassAccessor(typeid(dst));
			rca->LoadFromPtree(dst, src);
		}
		void ReflectiveCopyFromReflectionSystem(Reflectable& dst, const Reflectable& src) {
			auto rca = Reflection::GetRefrectableClassAccessor(typeid(dst));
			rca->Copy(dst, src);
		}
	}
}