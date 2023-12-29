#include "..\core\i_game_object.hpp"
#include "..\core\draw_system.hpp"
#include "planeta/core/Matrix2_2.hpp"
#include "planeta/core/LogUtility.hpp"
#include "planeta/core/ISceneInternal.hpp"

#include "CDraw2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//CDraw2D::Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDraw2D::Impl_ {
	public:
		std::unique_ptr<private_::CDraw2DManagerConnection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDraw2D
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDraw2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, draw_priority)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_scale)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, color)
			.shallow_copy_target(&CDraw2D::draw_priority_)
			.shallow_copy_target(&CDraw2D::position_)
			.shallow_copy_target(&CDraw2D::rotation_rad_)
			.shallow_copy_target(&CDraw2D::scale_)
			.shallow_copy_target(&CDraw2D::color_);
	}

	CDraw2D::CDraw2D(): impl_(std::make_unique<Impl_>()), draw_priority_(0), rotation_rad_(0.0), scale_(1.0, 1.0) { }

	CDraw2D::~CDraw2D() = default;

	void CDraw2D::Draw(ScreenDrawer2D &drawer) { DrawProc(drawer); }

	CDraw2D &CDraw2D::draw_priority(int priority) {
		draw_priority_ = priority;
		//ゲームオブジェクトがアクティブなら優先度更新
		UpdatePriority_();
		return *this;
	}

	Vector2Dd CDraw2D::GetDrawCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d_->rotation_rad(), position_);
		//ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d_->scale().x; //横方向拡大を反映
		relation_position.y *= transform2d_->scale().y; //縦方向拡大を反映
		return transform2d_->position() + relation_position;
	}

	double CDraw2D::GetDrawRotationRed() const { return transform2d_->rotation_rad() + rotation_rad_; }

	Vector2Dd CDraw2D::GetDrawScale() const {
		return Vector2Dd(transform2d_->scale().x * scale_.x, transform2d_->scale().y * scale_.y);
	}

	void CDraw2D::UpdatePriority_() {
		if (is_active()) { impl_->draw_system_connection->change_priority(draw_priority_); }
	}

	void CDraw2D::on_initialized() {
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_draw_2d(
			shared_this<CDraw2D>(), draw_priority_);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void CDraw2D::on_finalized() noexcept { impl_->draw_system_connection->remove(); }

	void CDraw2D::on_activated() {
		auto suceed = impl_->draw_system_connection->active();
		if (!suceed) { PE_LOG_ERROR("描画システムの有効化に失敗しました。"); }
	}

	void CDraw2D::on_inactivated() {
		impl_->draw_system_connection->inactivate();
		super::on_inactivated();
	}

	bool CDraw2D::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}
}
