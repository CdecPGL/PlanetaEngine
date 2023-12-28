#include "planeta/core/IGameObject.hpp"
#include "..\core\collision_world.hpp"
#include "planeta/core/LogUtility.hpp"
#include "planeta/core/ISceneInternal.hpp"
#include "planeta/core/Matrix2_2.hpp"
#include "..\core\collider_2d_data.hpp"
#include "planeta/core/e_collision_with_ground_2d.hpp"

#include "CCollider2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CCollider2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, relative_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, relative_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, is_collidable_with_ground)
			.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, collision_group)
			.read_only_property("is_grounded", &CCollider2D::is_grounded)
			.shallow_copy_target(&CCollider2D::position_)
			.shallow_copy_target(&CCollider2D::rotation_rad_)
			.shallow_copy_target(&CCollider2D::collide_with_ground_flag_)
			.shallow_copy_target(&CCollider2D::collision_group_name_);
	}

	void CCollider2D::on_activated() {
		super::on_activated();
		ResistToCollisionDetectProcess_();
	}

	void CCollider2D::on_inactivated() {
		RemoveFromCollisionDetectProcess_();
		super::on_inactivated();
	}

	void CCollider2D::ResistToCollisionDetectProcess_() {
		if (collision_group_name_.length() == 0) { PE_LOG_ERROR("衝突グループが設定されていません。"); } else {
			auto col_grng_eve = [&eve = collided_with_ground2d,&is_grounded_flag = is_grounded_](
				const e_collision_with_ground_2d &arg) {
				switch (arg.collision_state) {
					case collision_state::enter:
						is_grounded_flag = true;
						break;
					case collision_state::exit:
						is_grounded_flag = false;
						break;
					default:
						break;
				}
				eve(arg);
			};
			private_::collider_2d_data col_dat{
				*this, game_object(), *transform2d_,
				[&eve = collided_with_collider2d](const e_collision_with_collider_2d &arg) { eve(arg); }, col_grng_eve
			};
			scene_internal_interface().collision_world_internal_pointer()->resist(col_dat);
		}
	}

	void CCollider2D::RemoveFromCollisionDetectProcess_() {
		scene_internal_interface().collision_world_internal_pointer()->remove(this);
	}

	const Vector2Dd CCollider2D::GetCollisionGlobalCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d().rotation_rad(), position_);
		//ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d().scale().x; //横方向拡大を反映
		relation_position.y *= transform2d().scale().y; //縦方向拡大を反映
		return transform2d().position() + relation_position;
	}

	const double CCollider2D::GetCollisionScale() const {
		return transform2d().scale().x > transform2d().scale().y ? transform2d().scale().y : transform2d().scale().x;
	}

	const double CCollider2D::GetCollisionGlobalRotationRad() const {
		return transform2d().rotation_rad() + rotation_rad_;
	}

	CCollider2D &CCollider2D::collision_group(const std::string &cg) {
		if (is_active()) {
			//アクティブだったら衝突判定プロセスに変更での変更を行う。
			if (scene_internal_interface().collision_world_internal_pointer()->change_collision_group(this, cg)) {
				collision_group_name_ = cg;
			} else { PE_LOG_ERROR("衝突グループを", collision_group_name_, "から", cg, "に変更できませんでした。"); }
		} else { collision_group_name_ = cg; }
		return *this;
	}

	CCollider2D &CCollider2D::is_collidable_with_ground(bool flag) {
		if (is_active()) {
			//アクティブだったら衝突判定プロセスでの変更を行う。
			if (scene_internal_interface().collision_world_internal_pointer()->
			                               change_collision_with_ground_flag(this, flag)) {
				collide_with_ground_flag_ = flag;
			} else {
				PE_LOG_ERROR("地形との衝突フラグを", collide_with_ground_flag_ ? "true" : "false", "から", flag ? "true" : "false",
				             "に変更できませんでした。");
			}
		} else { collide_with_ground_flag_ = flag; }
		return *this;
	}

	bool CCollider2D::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}
}
