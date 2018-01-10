#include "planeta/ReflectableClassAccessor.hpp"
#include "ClassInfoCaller.hpp"
#include "Reflection.hpp"

namespace plnt {
	using namespace private_;
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class ReflectableClassAccessor::Impl_ {
	public:
		Impl_(const ClassInfo* ci)noexcept : ci_caller(*ci) {}
		ClassInfoCaller ci_caller;
	};

	//////////////////////////////////////////////////////////////////////////
	//Object
	//////////////////////////////////////////////////////////////////////////
	ReflectableClassAccessor::ReflectableClassAccessor(const ClassInfo* ci)noexcept :impl_(std::make_unique<Impl_>(ci)) {}

	ReflectableClassAccessor::~ReflectableClassAccessor()noexcept {}

	void ReflectableClassAccessor::SetVariable_(Reflectable& obj, const std::string& var_id, const boost::any& v) {
		impl_->ci_caller.SetVariable(var_id, obj, v);
	}

	void ReflectableClassAccessor::GetVariable_(Reflectable& obj, const std::string& var_id, boost::any& v) {
		impl_->ci_caller.GetVariable(var_id, obj, v);
	}

	std::shared_ptr<Reflectable> ReflectableClassAccessor::Clone(Reflectable& obj) {
		return impl_->ci_caller.Clone(obj);
	}

	void ReflectableClassAccessor::LoadFromPtree(Reflectable& obj, const boost::property_tree::ptree& pt) {
		try {
			impl_->ci_caller.SetDataFromPtree(pt, obj);
		} catch (reflection_error& e) {
			throw reflection_error(util::ConvertAndConnectToString("Ptreeからの読み込みにおいてエラーが発生しました。:", e.what()));
		}
	}

	void ReflectableClassAccessor::Copy(Reflectable& dst, const Reflectable& obj) {
		impl_->ci_caller.CopyFrom(dst, obj);
	}
}