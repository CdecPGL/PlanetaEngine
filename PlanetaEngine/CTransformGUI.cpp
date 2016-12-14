#include "CTransformGUI.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//CTransformGUI::Impl_
	//////////////////////////////////////////////////////////////////////////

	class CTransformGUI::Impl_ {
	public:
		Vector2Di position{ 0,0 };
		Vector2Di size{ 0,0 };
		double rotation_rad = 0;
		Vector2Dd pivot{ 0,0 };
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransformGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CTransformGUI) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransformGUI, position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransformGUI, size)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransformGUI, rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransformGUI, pivot)
			.DeepCopyTarget(&CTransformGUI::impl_);
	}

	CTransformGUI::CTransformGUI() :impl_(std::make_unique<Impl_>()) {}

	CTransformGUI::~CTransformGUI() = default;

	const planeta::Vector2Di& CTransformGUI::position() const {
		return impl_->position;
	}

	planeta::CTransformGUI& CTransformGUI::position(const Vector2Di& pos) {
		impl_->position = pos;
		return *this;
	}

	const double CTransformGUI::rotation_rad() const {
		return impl_->rotation_rad;
	}

	planeta::CTransformGUI& CTransformGUI::rotation_rad(double rota_rad) {
		impl_->rotation_rad = rota_rad;
		return *this;
	}

	planeta::CTransformGUI& CTransformGUI::size(const Vector2Di& s) {
		impl_->size = s;
		return *this;
	}

	const planeta::Vector2Di& CTransformGUI::size() const {
		return impl_->size;
	}

	planeta::CTransformGUI& CTransformGUI::pivot(const Vector2Dd& p) {
		impl_->pivot = p;
		return *this;
	}

	const planeta::Vector2Dd& CTransformGUI::pivot() const {
		return impl_->pivot;
	}

}