#include "ReflectionUtility.h"
#include "Reflection.h"
#include "Reflectable.h"
#include "ReflectableClassAccessor.h"

namespace planeta {
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