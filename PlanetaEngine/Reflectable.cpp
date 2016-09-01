#include "Reflectable.h"
#include "ClassInfoCaller.h"

namespace planeta {
	using namespace private_;
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class Reflectable::Impl_ {
	public:
		Impl_(Reflectable* obj)noexcept :obj_(obj) {}
		ClassInfoCaller class_info_caller()const {
			if (class_info_ == nullptr) {
				class_info_ = Reflection::GetClassInfo_Reflectable(typeid(*obj_));
				if (class_info_ == nullptr) {
					throw reflection_error(util::ConvertAndConnectToString("リフレクションシステムからクラス情報(typeinfo:", typeid(obj_).name(), ")を取得できませんでした。"));
				}
			}
			return ClassInfoCaller(*class_info_);
		}
		void ResetObj(Reflectable* obj) { obj_ = obj; }
	private:
		mutable const ClassInfo* class_info_ = nullptr;
		Reflectable* obj_ = nullptr;
	};


	//////////////////////////////////////////////////////////////////////////
	//Object
	//////////////////////////////////////////////////////////////////////////
	Reflectable::Reflectable()noexcept :impl_(std::make_unique<Impl_>(this)) {}

	Reflectable::Reflectable(const Reflectable& obj) noexcept : impl_(std::make_unique<Impl_>(*obj.impl_)) {
		impl_->ResetObj(this);
	}

	Reflectable::Reflectable(Reflectable&& obj) noexcept : impl_(std::move(obj.impl_)) {
		impl_->ResetObj(this);
		obj.impl_ = std::make_unique<Impl_>(&obj);
	}

	Reflectable::~Reflectable()noexcept {}

	Reflectable& Reflectable::operator=(const Reflectable& obj) noexcept {
		impl_ = std::make_unique<Impl_>(*obj.impl_);
		impl_->ResetObj(this);
		return *this;
	}

	Reflectable& Reflectable::operator=(Reflectable&& obj) noexcept {
		impl_ = std::move(obj.impl_);
		impl_->ResetObj(this);
		obj.impl_ = std::make_unique<Impl_>(&obj);
		return *this;
	}

	void Reflectable::SetVariable_(const std::string& var_id, const boost::any& v) {
		impl_->class_info_caller().SetVariable(var_id, *this, v);
	}

	void Reflectable::GetVariable_(const std::string& var_id, boost::any& v) {
		impl_->class_info_caller().GetVariable(var_id, *this, v);
	}

	std::shared_ptr<Reflectable> Reflectable::Clone() {
		return impl_->class_info_caller().Clone(*this);
	}

	void Reflectable::LoadFromPtree(const boost::property_tree::ptree& pt) {
		try {
			impl_->class_info_caller().SetDataFromPtree(pt, *this);
		} catch (reflection_error& e) {
			throw reflection_error(util::ConvertAndConnectToString("Ptreeからの読み込みにおいてエラーが発生しました。:", e.what()));
		}
	}

	void Reflectable::CopyFrom(const Reflectable& obj) {
		impl_->class_info_caller().CopyFrom(*this, obj);
	}
}