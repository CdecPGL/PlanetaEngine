#include "ReflectionAccessible.hpp"
#include "ClassInfoCaller.hpp"
#include "Reflection.hpp"
#include "Reflectable.hpp"

namespace plnt::reflection {
	using namespace private_;
	using namespace plnt::util;
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class ReflectionAccessible::Impl_ {
	public:
		Impl_(ReflectionAccessible* obj)noexcept : obj_(obj) {}
		ClassInfoCaller class_info_caller()const {
			if (class_info_ == nullptr) {
				class_info_ = Reflection::GetClassInfo_Reflectable(typeid(reflectable_obj()));
				if (class_info_ == nullptr) {
					throw reflection_error(ConvertAndConnectToString("���t���N�V�����V�X�e������N���X���(typeinfo:", typeid(reflectable_obj_).name(), ")���擾�ł��܂���ł����B"));
				}
			}
			return ClassInfoCaller(*class_info_);
		}
		void ResetObj(ReflectionAccessible* obj) {
			reflectable_obj_ = dynamic_cast<Reflectable*>(obj);
			obj_ = obj;
		}
		Reflectable& reflectable_obj()const {
			if (reflectable_obj_ == nullptr) {
				//�R���X�g���N�g���ɂ͂܂��^�����S�łȂ����߁AReflectable�|�C���^�̎擾�͂����ōs��
				reflectable_obj_ = dynamic_cast<Reflectable*>(obj_);
				if (reflectable_obj_ == nullptr) {
					throw reflection_error(ConvertAndConnectToString("Reflectable�N���X�̃|�C���^���Z�b�g����Ă��܂���BReflectable�N���X���p�����Ă��Ȃ��\��������܂��BReflectionAccessible�̋@�\�𗘗p����ɂ́AReflectable���p�����Ă��Ȃ���΂Ȃ�܂���B"));
				}
			}
			return *reflectable_obj_; 
		}
	private:
		mutable const ClassInfo* class_info_ = nullptr;
		mutable Reflectable* reflectable_obj_ = nullptr;
		ReflectionAccessible* obj_ = nullptr;
	};


	//////////////////////////////////////////////////////////////////////////
	//Object
	//////////////////////////////////////////////////////////////////////////
	ReflectionAccessible::ReflectionAccessible()noexcept :impl_(std::make_unique<Impl_>(this)) {}

	ReflectionAccessible::ReflectionAccessible(const ReflectionAccessible& obj) noexcept : impl_(std::make_unique<Impl_>(*obj.impl_)) {
		impl_->ResetObj(this);
	}

	ReflectionAccessible::ReflectionAccessible(ReflectionAccessible&& obj) noexcept : impl_(std::move(obj.impl_)) {
		impl_->ResetObj(this);
		obj.impl_ = std::make_unique<Impl_>(&obj);
	}

	ReflectionAccessible::~ReflectionAccessible() noexcept {}

	ReflectionAccessible& ReflectionAccessible::operator=(const ReflectionAccessible& obj) noexcept {
		impl_ = std::make_unique<Impl_>(*obj.impl_);
		impl_->ResetObj(this);
		return *this;
	}

	ReflectionAccessible& ReflectionAccessible::operator=(ReflectionAccessible&& obj) noexcept {
		impl_ = std::move(obj.impl_);
		impl_->ResetObj(this);
		obj.impl_ = std::make_unique<Impl_>(&obj);
		return *this;
	}

	void ReflectionAccessible::SetVariable_(const std::string& var_id, const boost::any& v) {
		impl_->class_info_caller().SetVariable(var_id, impl_->reflectable_obj(), v);
	}

	void ReflectionAccessible::GetVariable_(const std::string& var_id, boost::any& v) {
		impl_->class_info_caller().GetVariable(var_id, impl_->reflectable_obj(), v);
	}

	std::shared_ptr<Reflectable> ReflectionAccessible::ReflectiveClone() {
		return impl_->class_info_caller().Clone(impl_->reflectable_obj());
	}

	void ReflectionAccessible::ReflectiveLoadFromPtree(const boost::property_tree::ptree& pt) {
		try {
			impl_->class_info_caller().SetDataFromPtree(pt, impl_->reflectable_obj());
		} catch (reflection_error& e) {
			throw reflection_error(ConvertAndConnectToString("Ptree����̓ǂݍ��݂ɂ����ăG���[���������܂����B:", e.what()));
		}
	}

	void ReflectionAccessible::ReflectiveCopyFrom(const Reflectable& obj) {
		impl_->class_info_caller().CopyFrom(impl_->reflectable_obj(), obj);
	}
}