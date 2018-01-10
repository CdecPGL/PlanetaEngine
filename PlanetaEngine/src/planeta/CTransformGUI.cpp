#include <unordered_map>

#include "planeta/CTransformGUI.hpp"
#include "ISceneInternal.hpp"
#include "TransformSystem.hpp"
#include "LogUtility.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//CTransformGUI::Impl_
	//////////////////////////////////////////////////////////////////////////

	class CTransformGUI::Impl_ {
	public:
		RectAnglei rect;
		Vector2Di position;
		Vector2Di size{ 1,1 };
		double rotation_rad = 0;
		Vector2Dd pivot{ 0.5,0.5 };
		Impl_& operator=(const Impl_& imp) {
			rect = imp.rect;
			position = imp.position;
			size = imp.size;
			rotation_rad = imp.rotation_rad;
			pivot = imp.pivot;
			return *this;
		}

		int t2d_id_ = -1;
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

	const plnt::Vector2Di& CTransformGUI::position() const {
		return impl_->position;
	}

	plnt::CTransformGUI& CTransformGUI::position(const Vector2Di& pos) {
		impl_->position = pos;
		return *this;
	}

	const double CTransformGUI::rotation_rad() const {
		return impl_->rotation_rad;
	}

	plnt::CTransformGUI& CTransformGUI::rotation_rad(double rota_rad) {
		impl_->rotation_rad = rota_rad;
		return *this;
	}

	plnt::CTransformGUI& CTransformGUI::size(const Vector2Di& s) {
		impl_->size = s;
		return *this;
	}

	const plnt::Vector2Di& CTransformGUI::size() const {
		return impl_->size;
	}

	plnt::CTransformGUI& CTransformGUI::pivot(const Vector2Dd& p) {
		impl_->pivot = p;
		return *this;
	}

	const plnt::Vector2Dd& CTransformGUI::pivot() const {
		return impl_->pivot;
	}

	void CTransformGUI::OnActivated() {
		Super::OnActivated();
		//TransformSystem‚Ö“o˜^
		impl_->t2d_id_ = scene_internal_interface().transform_system_internal_pointer()->RegisterTransformGUI(this);
		PE_VERIFY(impl_->t2d_id_ >= 0);
	}

	void CTransformGUI::OnInactivated() {
		PE_VERIFY(impl_->t2d_id_ >= 0);
		//TransformSystem‚©‚ç“o˜^‰ðœ
		if (!scene_internal_interface().transform_system_internal_pointer()->RemoveTransformGUI(impl_->t2d_id_)) {
			PE_LOG_FATAL("TransfromSystem‚©‚ç‚Ì“o˜^‰ðœ‚ÉŽ¸”s‚µ‚Ü‚µ‚½BID:", impl_->t2d_id_);
		}
		Super::OnInactivated();
	}

	/*const planeta::RectAnglei& CTransformGUI::rect() const {
		return impl_->rect;
	}*/

}