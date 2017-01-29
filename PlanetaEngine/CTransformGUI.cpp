#include <unordered_map>

#include "CTransformGUI.h"
#include "ISceneInternal.h"
#include "TransformSystem.h"
#include "LogUtility.h"

namespace planeta {
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

	bool CTransformGUI::OnActivated() {
		if (!Super::OnActivated()) { return false; }
		//TransformSystemへ登録
		impl_->t2d_id_ = scene_internal_interface().transform_system_internal_pointer()->RegisterTransformGUI(this);
		PE_VERIFY(impl_->t2d_id_ >= 0);
		return true;
	}

	void CTransformGUI::OnInactivated() {
		PE_VERIFY(impl_->t2d_id_ >= 0);
		//TransformSystemから登録解除
		if (!scene_internal_interface().transform_system_internal_pointer()->RemoveTransformGUI(impl_->t2d_id_)) {
			PE_LOG_FATAL("TransfromSystemからの登録解除に失敗しました。ID:", impl_->t2d_id_);
		}
		Super::OnInactivated();
	}

	const planeta::RectAnglei& CTransformGUI::rect() const {
		return impl_->rect;
	}

}