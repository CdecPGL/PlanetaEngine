#include "CDraw2D.h"
#include "IGameObjectForComponent.h"
//#include "GameObjectDrawComponentProcessRegistrator.h"
#include "GameObjectDrawSystem.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "SceneDataForGameObject.h"

namespace planeta_engine {
	CDraw2D::CDraw2D() :draw_priority_(0), rotation_rad_(0.0), scale_(1.0, 1.0) {

	}

	CDraw2D& CDraw2D::draw_priority(int priority) {
		draw_priority_ = priority;
		//ゲームオブジェクトがアクティブなら優先度更新
		UpdatePriority_();
		return *this;
	}

	Vector2Dd CDraw2D::GetDrawCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d_->rotation_rad(), position_); //ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d_->scale().x; //横方向拡大を反映
		relation_position.y *= transform2d_->scale().y; //縦方向拡大を反映
		return transform2d_->position() + relation_position;
	}

	double CDraw2D::GetDrawRotationRed() const {
		return transform2d_->rotation_rad() + rotation_rad_;
	}

	Vector2Dd CDraw2D::GetDrawScale() const {
		return Vector2Dd(transform2d_->scale().x * scale_.x, transform2d_->scale().y * scale_.y);
	}

	void CDraw2D::RegisterToProcess_() {
		scene_data_ref().gameobject_draw_system.Register(std::static_pointer_cast<CDraw2D>(shared_from_this()), draw_priority_);
	}

	void CDraw2D::RemoveFromProcess_() {
		scene_data_ref().gameobject_draw_system.Remove(std::static_pointer_cast<CDraw2D>(shared_from_this()));
	}

	void CDraw2D::UpdatePriority_() {
		if (is_active()) {
			scene_data_ref().gameobject_draw_system.ChangePriority(std::static_pointer_cast<CDraw2D>(shared_from_this()), draw_priority_);
		}
	}

	bool CDraw2D::OnActivated() {
		RegisterToProcess_();
		return true;
	}

	bool CDraw2D::OnInactivated() {
		RemoveFromProcess_();
		return true;
	}

	bool CDraw2D::OnInitialized() {
		transform2d_.reset(game_object().GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}

}