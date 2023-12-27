#include "reflectable_class_accessor.hpp"
#include "class_info_caller.hpp"
#include "reflection.hpp"

namespace plnt::reflection {
	using namespace private_;
	using namespace util;
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class reflectable_class_accessor::impl {
	public:
		explicit impl(const class_info *ci) noexcept : ci_caller(*ci) { }

		class_info_caller ci_caller;
	};

	//////////////////////////////////////////////////////////////////////////
	//Object
	//////////////////////////////////////////////////////////////////////////
	reflectable_class_accessor::reflectable_class_accessor(const class_info *ci) noexcept : impl_(
		std::make_unique<impl>(ci)) { }

	reflectable_class_accessor::~reflectable_class_accessor() noexcept { }

	void reflectable_class_accessor::set_variable(reflectable &obj, const std::string &var_id, const boost::any &v) const {
		impl_->ci_caller.set_variable(var_id, obj, v);
	}

	void reflectable_class_accessor::get_variable(reflectable &obj, const std::string &var_id, boost::any &v) const {
		impl_->ci_caller.get_variable(var_id, obj, v);
	}

	std::shared_ptr<reflectable> reflectable_class_accessor::clone(reflectable &obj) const {
		return impl_->ci_caller.clone(obj);
	}

	void reflectable_class_accessor::load_from_ptree(reflectable &obj, const boost::property_tree::ptree &pt) const {
		try { impl_->ci_caller.set_data_from_ptree(pt, obj); } catch (reflection_error &e) {
			throw reflection_error(ConvertAndConnectToString("Ptreeからの読み込みにおいてエラーが発生しました。:", e.what()));
		}
	}

	void reflectable_class_accessor::copy(reflectable &dst, const reflectable &src) const {
		impl_->ci_caller.copy_from(dst, src);
	}
}
