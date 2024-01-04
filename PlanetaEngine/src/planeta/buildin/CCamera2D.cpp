#include "../core/draw_system.hpp"
#include "planeta/core/i_scene_internal.hpp"
#include "../core/log_utility.hpp"
#include "../core/i_game_object.hpp"
#include "CCamera2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//CCamera2D::Impl_
	//////////////////////////////////////////////////////////////////////////

	class c_camera_2d::impl {
	public:
		std::unique_ptr<private_::c_camera_2d_manager_connection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CCamera2D
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_camera_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_camera_2d, expansion)
			.shallow_copy_target(&c_camera_2d::expansion_);
	}

	c_camera_2d::c_camera_2d(): impl_(std::make_unique<impl>()), transform2d_() {}

	c_camera_2d::~c_camera_2d() = default;

	bool c_camera_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<c_transform_2d>());
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void c_camera_2d::on_initialized() {
		super::on_initialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_camera_2d(
			shared_this<c_camera_2d>());
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void c_camera_2d::on_activated() { }

	void c_camera_2d::on_inactivated() {
		PE_LOG_WARNING("カメラ2Dコンポーネントは無効化に対応していません。");
		super::on_inactivated();
	}

	void c_camera_2d::on_finalized() noexcept { impl_->draw_system_connection->remove(); }

	double c_camera_2d::expansion() const { return expansion_; }

	void c_camera_2d::expansion(const double e) { expansion_ = e; }

	const vector_2dd &c_camera_2d::position() const { return transform2d_->position(); }

	void c_camera_2d::position(const vector_2dd &p) const { transform2d_->position(p); }

	double c_camera_2d::rotation_rad() const { return transform2d_->rotation_rad(); }

	void c_camera_2d::rotation_rad(const double r) const { transform2d_->rotation_rad(r); }
}
