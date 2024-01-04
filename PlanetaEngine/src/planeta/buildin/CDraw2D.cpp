#include "../core/i_game_object.hpp"
#include "../core/draw_system.hpp"
#include "../core/matrix_22.hpp"
#include "../core/log_utility.hpp"
#include "planeta/core/i_scene_internal.hpp"
#include "CDraw2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//CDraw2D::Impl_
	//////////////////////////////////////////////////////////////////////////

	class c_draw_2d::impl {
	public:
		std::unique_ptr<private_::c_draw_2d_manager_connection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDraw2D
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_2d, draw_priority)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_2d, relative_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_2d, relative_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_2d, relative_scale)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_2d, color)
			.shallow_copy_target(&c_draw_2d::draw_priority_)
			.shallow_copy_target(&c_draw_2d::position_)
			.shallow_copy_target(&c_draw_2d::rotation_rad_)
			.shallow_copy_target(&c_draw_2d::scale_)
			.shallow_copy_target(&c_draw_2d::color_);
	}

	c_draw_2d::c_draw_2d(): transform2d_(), impl_(std::make_unique<impl>()), scale_(1.0, 1.0) {}

	c_draw_2d::~c_draw_2d() = default;

	void c_draw_2d::draw(screen_drawer_2d &drawer) { draw_proc(drawer); }

	c_draw_2d &c_draw_2d::draw_priority(const int priority) {
		draw_priority_ = priority;
		//ゲームオブジェクトがアクティブなら優先度更新
		update_priority();
		return *this;
	}

	vector_2dd c_draw_2d::get_draw_center_position() const {
		vector_2dd relation_position = math::rotation_transform(transform2d_->rotation_rad(), position_);
		//ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d_->scale().x; //横方向拡大を反映
		relation_position.y *= transform2d_->scale().y; //縦方向拡大を反映
		return transform2d_->position() + relation_position;
	}

	double c_draw_2d::get_draw_rotation_red() const { return transform2d_->rotation_rad() + rotation_rad_; }

	vector_2dd c_draw_2d::get_draw_scale() const {
		return {transform2d_->scale().x * scale_.x, transform2d_->scale().y * scale_.y};
	}

	void c_draw_2d::update_priority() const {
		if (is_active()) { impl_->draw_system_connection->change_priority(draw_priority_); }
	}

	void c_draw_2d::on_initialized() {
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_draw_2d(
			shared_this<c_draw_2d>(), draw_priority_);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void c_draw_2d::on_finalized() noexcept { impl_->draw_system_connection->remove(); }

	void c_draw_2d::on_activated() {
		if (const auto succeeded = impl_->draw_system_connection->active(); !succeeded) {
			PE_LOG_ERROR("描画システムの有効化に失敗しました。");
		}
	}

	void c_draw_2d::on_inactivated() {
		impl_->draw_system_connection->inactivate();
		super::on_inactivated();
	}

	bool c_draw_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<c_transform_2d>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}
}
