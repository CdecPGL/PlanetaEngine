#include "CCamera2D.h"
#include "DrawSystem.h"
#include "ISceneInternal.h"
#include "LogUtility.h"
#include "IGameObject.h"
#include "CTransform2D.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//CCamera2D::Impl_
	//////////////////////////////////////////////////////////////////////////

	class CCamera2D::Impl_ {
	public:
		std::unique_ptr<private_::CCamera2DManagerConnection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CCamera2D
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CCamera2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CCamera2D, expansion)
			.ShallowCopyTarget(&CCamera2D::expansion_);
	}

	CCamera2D::CCamera2D():impl_(std::make_unique<Impl_>()){}
	CCamera2D::~CCamera2D() = default;

	bool CCamera2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	bool CCamera2D::OnInitialized() {
		if (!Super::OnInitialized()) { return false; }
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCCamera2D(std::static_pointer_cast<CCamera2D>(shared_from_this()));
		return impl_->draw_system_connection != nullptr;
	}

	bool CCamera2D::OnActivated() {
		return true;
	}

	bool CCamera2D::OnInactivated() {
		PE_LOG_WARNING("カメラ2Dコンポーネントは無効化に対応していません。");
		return true;
	}

	void CCamera2D::OnFinalized() noexcept {
		impl_->draw_system_connection->Remove();
	}

	double CCamera2D::expansion() const {
		return expansion_;
	}

	void CCamera2D::expansion(double s) {
		expansion_ = s;
	}

	const planeta::Vector2Dd& CCamera2D::position() const {
		return transform2d_->position();
	}

	void CCamera2D::position(const planeta::Vector2Dd& p) {
		transform2d_->position(p);
	}

	double CCamera2D::rotation_rad() const {
		return transform2d_->rotation_rad();
	}

	void CCamera2D::rotation_rad(double r) {
		transform2d_->rotation_rad(r);
	}

}