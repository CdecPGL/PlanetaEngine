#include "..\core\draw_system.hpp"
#include "planeta/core/i_scene_internal.hpp"
#include "planeta/core/LogUtility.hpp"
#include "..\core\i_game_object.hpp"

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
			.shallow_copy_target(&CCamera2D::expansion_);
	}

	CCamera2D::CCamera2D(): impl_(std::make_unique<Impl_>()) { }

	CCamera2D::~CCamera2D() = default;

	bool CCamera2D::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void CCamera2D::on_initialized() {
		super::on_initialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_camera_2d(
			shared_this<CCamera2D>());
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void CCamera2D::on_activated() { }

	void CCamera2D::on_inactivated() {
		PE_LOG_WARNING("カメラ2Dコンポーネントは無効化に対応していません。");
		super::on_inactivated();
	}

	void CCamera2D::on_finalized() noexcept { impl_->draw_system_connection->remove(); }

	double CCamera2D::expansion() const { return expansion_; }

	void CCamera2D::expansion(double s) { expansion_ = s; }

	const plnt::Vector2Dd &CCamera2D::position() const { return transform2d_->position(); }

	void CCamera2D::position(const plnt::Vector2Dd &p) { transform2d_->position(p); }

	double CCamera2D::rotation_rad() const { return transform2d_->rotation_rad(); }

	void CCamera2D::rotation_rad(double r) { transform2d_->rotation_rad(r); }
}
