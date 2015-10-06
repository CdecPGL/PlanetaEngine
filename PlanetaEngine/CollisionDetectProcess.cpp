#include "CollisionDetectProcess.h"
#include "boost/next_prior.hpp"
#include "IGameObjectAccessor.h"
#include "CollisionDetectFunctions.h"
#include "CircleColliderComponent.h"
#include "StraightLineColliderComponent.h"
#include "GroundComponent.h"
#include "CollisionEventArgument.h"
#include "CollisionWithGroundEventArgument.h"

namespace planeta_engine{
	namespace system_processes {
		CollisionDetectProcess::~CollisionDetectProcess()
		{
		}

		void CollisionDetectProcess::Update()
		{
			_circle_collision_component_holder.Update();
			_straightline_collision_component_holder.Update();

			auto& circle_collider_list = _circle_collision_component_holder.GetCollisionComponentList();
			auto& straightline_collider_list = _straightline_collision_component_holder.GetCollisionComponentList();

			for (auto ccc_it = circle_collider_list.begin(); ccc_it != circle_collider_list.end(); ++ccc_it) {
				/*Circleと地形*/
				if ((*ccc_it)->collide_with_ground_flag()) {
					if ((*ccc_it)->game_object()->belonging_ground().CollisionDetect(*(*ccc_it))) {
						//衝突していたら地形衝突イベントを発生させる
						event_arguments::CollisionWithGroundEventArgument cwgea;
						(*ccc_it)->collided_with_ground(cwgea);
					}
				}
				/*CircleとCircle*/
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != circle_collider_list.end(); ++ccc_it2) {
					if (collision_map_.Get((*ccc_it)->collision_group(),(*ccc_it2)->collision_group(),true) && game::collision_detect_functions::CollisionDetectCircleAndCirecle(**ccc_it, **ccc_it2)) {
						//衝突していたら衝突イベントを発生させる
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						(*ccc_it)->collided(cea0);
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						(*ccc_it2)->collided(cea1);
					}
				}
				/*CircleとStraightLine*/
				for (auto slcc_it = straightline_collider_list.begin(); slcc_it != straightline_collider_list.end(); ++slcc_it) {
					if (collision_map_.Get((*ccc_it)->collision_group(), (*slcc_it)->collision_group(), true) && game::collision_detect_functions::CollisionDetectCircleAndStraightLine(**ccc_it, **slcc_it)) {
						//衝突していたら衝突イベントを発生させる
						event_arguments::CollisionEventArgument cea0((*slcc_it)->game_object());
						(*ccc_it)->collided(cea0);
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						(*slcc_it)->collided(cea1);
					}
				}
			}
			/*StraightLineと地形*/
			for (auto slcc_it = straightline_collider_list.begin(); slcc_it != straightline_collider_list.end(); ++slcc_it) {
				if ((*slcc_it)->collide_with_ground_flag()) {
					if ((*slcc_it)->game_object()->belonging_ground().CollisionDetect(*(*slcc_it))) {
						//衝突していたら地形衝突イベントを発生させる
						event_arguments::CollisionWithGroundEventArgument cwgea;
						(*slcc_it)->collided_with_ground(cwgea);
					}
				}
			}
		}
	}
}

