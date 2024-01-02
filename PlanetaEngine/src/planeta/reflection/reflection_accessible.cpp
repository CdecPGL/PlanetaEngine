#include "reflection_accessible.hpp"
#include "class_info_caller.hpp"
#include "reflection.hpp"
#include "reflectable.hpp"

namespace plnt::reflection {
	using namespace private_;
	using namespace util;
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class reflection_accessible::impl {
	public:
		explicit impl(reflection_accessible *obj) noexcept : obj_(obj) { }

		class_info_caller class_info_caller() const {
			if (class_info_ == nullptr) {
				decltype (auto) ref_obj = reflectable_obj();
				class_info_ = reflection::get_class_info_reflectable(typeid(ref_obj));
				if (class_info_ == nullptr) {
					throw reflection_error(convert_and_connect_to_string("リフレクションシステムからクラス情報(typeinfo:",
					                                                 typeid(reflectable_obj_).name(), ")を取得できませんでした。"));
				}
			}
			return plnt::reflection::class_info_caller{*class_info_};
		}

		void reset_obj(reflection_accessible *obj) {
			reflectable_obj_ = dynamic_cast<reflectable *>(obj);
			obj_ = obj;
		}

		reflectable &reflectable_obj() const {
			if (reflectable_obj_ == nullptr) {
				//コンストラクト時にはまだ型が完全でないため、Reflectableポインタの取得はここで行う
				reflectable_obj_ = dynamic_cast<reflectable *>(obj_);
				if (reflectable_obj_ == nullptr) {
					throw reflection_error(convert_and_connect_to_string(
						"Reflectableクラスのポインタがセットされていません。Reflectableクラスを継承していない可能性があります。ReflectionAccessibleの機能を利用するには、Reflectableを継承していなければなりません。"));
				}
			}
			return *reflectable_obj_;
		}

	private:
		mutable const class_info *class_info_ = nullptr;
		mutable reflectable *reflectable_obj_ = nullptr;
		reflection_accessible *obj_ = nullptr;
	};


	//////////////////////////////////////////////////////////////////////////
	//Object
	//////////////////////////////////////////////////////////////////////////
	reflection_accessible::reflection_accessible() noexcept : impl_(std::make_unique<impl>(this)) { }

	reflection_accessible::reflection_accessible(const reflection_accessible &obj) noexcept : impl_(
		std::make_unique<impl>(*obj.impl_)) { impl_->reset_obj(this); }

	reflection_accessible::reflection_accessible(reflection_accessible &&obj) noexcept : impl_(std::move(obj.impl_)) {
		impl_->reset_obj(this);
		obj.impl_ = std::make_unique<impl>(&obj);
	}

	reflection_accessible::~reflection_accessible() noexcept = default;

	reflection_accessible &reflection_accessible::operator=(const reflection_accessible &obj) noexcept {
		impl_ = std::make_unique<impl>(*obj.impl_);
		impl_->reset_obj(this);
		return *this;
	}

	reflection_accessible &reflection_accessible::operator=(reflection_accessible &&obj) noexcept {
		impl_ = std::move(obj.impl_);
		impl_->reset_obj(this);
		obj.impl_ = std::make_unique<impl>(&obj);
		return *this;
	}

	void reflection_accessible::set_variable(const std::string &var_id, const boost::any &v) const {
		impl_->class_info_caller().set_variable(var_id, impl_->reflectable_obj(), v);
	}

	void reflection_accessible::get_variable(const std::string &var_id, boost::any &v) const {
		impl_->class_info_caller().get_variable(var_id, impl_->reflectable_obj(), v);
	}

	std::shared_ptr<reflectable> reflection_accessible::reflective_clone() const {
		return impl_->class_info_caller().clone(impl_->reflectable_obj());
	}

	void reflection_accessible::reflective_load_from_ptree(const boost::property_tree::ptree &pt) const {
		try { impl_->class_info_caller().set_data_from_ptree(pt, impl_->reflectable_obj()); } catch (reflection_error &
			e) { throw reflection_error(convert_and_connect_to_string("Ptreeからの読み込みにおいてエラーが発生しました。:", e.what())); }
	}

	void reflection_accessible::reflective_copy_from(const reflectable &src) const {
		impl_->class_info_caller().copy_from(impl_->reflectable_obj(), src);
	}
}
