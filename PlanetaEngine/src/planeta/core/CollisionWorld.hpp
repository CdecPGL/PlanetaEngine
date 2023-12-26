#pragma once

#include "ICollisionWorld.hpp"
#include "SceneModule.hpp"

namespace plnt {
	class CCollider2D;

	namespace private_ {
		struct Collider2DData;

		class CollisionWorld : public ICollisionWorld, public SceneModule {
		public:
			virtual ~CollisionWorld() = 0 { };
			virtual bool Resist(const Collider2DData &collider_data) = 0;
			virtual bool Remove(const CCollider2D *col_com_ptr) = 0;
			virtual bool ChangeCollisionGroup(const CCollider2D *col_com_ptr, const std::string &group_name) = 0;
			virtual bool ChangeCollisionWithGroundFlag(const CCollider2D *col_com_ptr, bool flag) = 0;
			virtual void ExcuteCollisionDetection() = 0;
		};
	}
}
