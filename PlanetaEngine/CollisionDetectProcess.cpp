#include "CollisionDetectProcess.h"
#include "boost/next_prior.hpp"
#include "IGameObjectAccessor.h"
#include "CollisionDetectFunctions.h"
#include "ColliderComponent.h"
#include "GroundComponent.h"
#include "CollisionEventArgument.h"
#include "CollisionWithGroundEventArgument.h"
#include "CollisionGroupMatrix.h"

namespace planeta_engine{
	namespace system_processes {
		CollisionDetectProcess::~CollisionDetectProcess()
		{
		}

		void CollisionDetectProcess::Update()
		{
			collider_component_holder_.Update();

			auto& collider_list = collider_component_holder_.GetCollisionComponentList();

			for (auto ccc_it = collider_list.begin(); ccc_it != collider_list.end(); ++ccc_it) {
				/*地形と*/
				if ((*ccc_it)->collide_with_ground_flag()) {
					if ((*ccc_it)->DetectCollision((*ccc_it)->game_object().belonging_ground())) {
						//衝突していたら地形衝突イベントを発生させる
						event_arguments::CollisionWithGroundEventArgument cwgea;
						(*ccc_it)->collided_with_ground(cwgea);
					}
				}
				/*コライダー同士*/
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != collider_list.end(); ++ccc_it2) {
					if (collision_group_matrix_->IsCollision((*ccc_it)->collision_group_id(),(*ccc_it2)->collision_group_id()) && (*ccc_it)->DetectCollision(**ccc_it2)) {
						//衝突していたら衝突イベントを発生させる
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						(*ccc_it)->collided(cea0);
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						(*ccc_it2)->collided(cea1);
					}
				}
			}
		}
	}
}

