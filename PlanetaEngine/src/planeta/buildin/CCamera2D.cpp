#include "planeta/core/DrawSystem.hpp"
#include "planeta/core/ISceneInternal.hpp"
#include "planeta/core/LogUtility.hpp"
#include "planeta/core/IGameObject.hpp"

#include "CCamera2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
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

	CCamera2D::CCamera2D(): impl_(std::make_unique<Impl_>()) { }

	CCamera2D::~CCamera2D() = default;

	bool CCamera2D::GetOtherComponentsProc(const GOComponentGetter &com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void CCamera2D::OnInitialized() {
		Super::OnInitialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCCamera2D(
			shared_this<CCamera2D>());
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void CCamera2D::OnActivated() { }

	void CCamera2D::OnInactivated() {
		PE_LOG_WARNING("カメラ2Dコンポーネントは無効化に対応していません。");
		Super::OnInactivated();
	}

	void CCamera2D::OnFinalized() noexcept { impl_->draw_system_connection->Remove(); }

	double CCamera2D::expansion() const { return expansion_; }

	void CCamera2D::expansion(double s) { expansion_ = s; }

	const plnt::Vector2Dd &CCamera2D::position() const { return transform2d_->position(); }

	void CCamera2D::position(const plnt::Vector2Dd &p) { transform2d_->position(p); }

	double CCamera2D::rotation_rad() const { return transform2d_->rotation_rad(); }

	void CCamera2D::rotation_rad(double r) { transform2d_->rotation_rad(r); }
}
