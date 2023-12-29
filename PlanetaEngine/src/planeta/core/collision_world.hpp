#pragma once

#include "i_collision_world.hpp"
#include "SceneModule.hpp"

namespace plnt {
	class CCollider2D;

	namespace private_ {
		struct collider_2d_data;

		class collision_world : public i_collision_world, public SceneModule {
		public:
			collision_world() = default;
			collision_world(const collision_world &) = delete;
			collision_world(collision_world &&) = delete;
			// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition, modernize-use-equals-default)
			~collision_world() override = 0 { }
			collision_world &operator=(const collision_world &) = delete;
			collision_world &operator=(collision_world &&) = delete;

			virtual bool resist(const collider_2d_data &collider_data) = 0;
			virtual bool remove(const CCollider2D *col_com_ptr) = 0;
			virtual bool change_collision_group(const CCollider2D *col_com_ptr, const std::string &group_name) = 0;
			virtual bool change_collision_with_ground_flag(const CCollider2D *col_com_ptr, bool flag) = 0;
			virtual void execute_collision_detection() = 0;
		};
	}
}
