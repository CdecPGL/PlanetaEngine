#include "../core/i_game_object.hpp"
#include "../core/collision_world.hpp"
#include "../core/log_utility.hpp"
#include "planeta/core/i_scene_internal.hpp"
#include "../core/matrix_22.hpp"
#include "../core/collider_2d_data.hpp"
#include "planeta/core/e_collision_with_ground_2d.hpp"
#include "c_collider_2d.hpp"
#include "c_transform_2d.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_collider_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_collider_2d, relative_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_collider_2d, relative_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_collider_2d, is_collidable_with_ground)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_collider_2d, collision_group)
			.read_only_property("is_grounded", &c_collider_2d::is_grounded)
			.shallow_copy_target(&c_collider_2d::position_)
			.shallow_copy_target(&c_collider_2d::rotation_rad_)
			.shallow_copy_target(&c_collider_2d::collide_with_ground_flag_)
			.shallow_copy_target(&c_collider_2d::collision_group_name_);
	}

	void c_collider_2d::on_activated() {
		super::on_activated();
		resist_to_collision_detect_process();
	}

	void c_collider_2d::on_inactivated() {
		remove_from_collision_detect_process();
		super::on_inactivated();
	}

	void c_collider_2d::resist_to_collision_detect_process() {
		if (collision_group_name_.length() == 0) { PE_LOG_ERROR("衝突グループが設定されていません。"); } else {
			auto col_ground_eve = [&eve = collided_with_ground2d,&is_grounded_flag = is_grounded_](
				const e_collision_with_ground_2d &arg) {
				switch (arg.collision_state) {
					case collision_state::enter:
						is_grounded_flag = true;
						break;
					case collision_state::exit:
						is_grounded_flag = false;
						break;
					case collision_state::stay:
						break;
				}
				eve(arg);
			};
			const private_::collider_2d_data col_dat{
				*this, game_object(), *transform2d_,
				[&eve = collided_with_collider2d](const e_collision_with_collider_2d &arg) { eve(arg); }, col_ground_eve
			};
			scene_internal_interface().collision_world_internal_pointer()->resist(col_dat);
		}
	}

	void c_collider_2d::remove_from_collision_detect_process() {
		scene_internal_interface().collision_world_internal_pointer()->remove(this);
	}

	vector_2dd c_collider_2d::get_collision_global_center_position() const {
		vector_2dd relation_position = math::rotation_transform(transform2d().rotation_rad(), position_);
		//ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d().scale().x; //横方向拡大を反映
		relation_position.y *= transform2d().scale().y; //縦方向拡大を反映
		return transform2d().position() + relation_position;
	}

	double c_collider_2d::get_collision_scale() const {
		return transform2d().scale().x > transform2d().scale().y ? transform2d().scale().y : transform2d().scale().x;
	}

	double c_collider_2d::get_collision_global_rotation_rad() const {
		return transform2d().rotation_rad() + rotation_rad_;
	}

	c_collider_2d &c_collider_2d::collision_group(const std::string &cg) {
		if (is_active()) {
			//アクティブだったら衝突判定プロセスに変更での変更を行う。
			if (scene_internal_interface().collision_world_internal_pointer()->change_collision_group(this, cg)) {
				collision_group_name_ = cg;
			} else { PE_LOG_ERROR("衝突グループを", collision_group_name_, "から", cg, "に変更できませんでした。"); }
		} else { collision_group_name_ = cg; }
		return *this;
	}

	c_collider_2d &c_collider_2d::is_collidable_with_ground(bool flag) {
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

	bool c_collider_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<c_transform_2d>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}
}
