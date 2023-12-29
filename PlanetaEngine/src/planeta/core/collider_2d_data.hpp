#pragma once

#include <functional>

namespace plnt {
	class CCollider2D;
	class i_game_object;
	class CTransform2D;
	class e_collision_with_collider_2d;
	class e_collision_with_ground_2d;

	namespace private_ {
		//このデータがCollisionWorldで保持されている間はそのColiderは存在しているはずなので、メンバに参照を含んでもよい。
		struct collider_2d_data {
			CCollider2D &collider2d;
			i_game_object &game_object;
			CTransform2D &transform2d;
			std::function<void(const e_collision_with_collider_2d &)> collide_with_collider_event_invoker;
			std::function<void(const e_collision_with_ground_2d &)> collide_with_ground_event_invoker;
		};
	}
}
