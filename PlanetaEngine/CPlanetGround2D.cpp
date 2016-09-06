#include "CPlanetGround2D.h"
#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "CCircleCollider2D.h"
#include "CPlanet.h"
#include "SystemLog.h"
#include "MathConstant.h"

namespace planeta {
	bool CPlanetGround2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!CGround2D::GetOtherComponentsProc(com_getter)) { return false; };
		planet_component_.reset(com_getter.GetComponent<CPlanet>());
		if (!planet_component_) {
			PE_LOG_ERROR("PlanetComponentを取得できませんでした。");
			return false;
		} else {
			return true;
		}
	}

	bool CPlanetGround2D::CollideWith(CCircleCollider2D& collider) {
		//			TransformComponent& transform = game_object().transform();
		auto collider_pos = collider.GetCollisionGlobalCenterPosition();
		//コライダーの惑星座標を求める
		auto collider_ground_pos = ConvertPositionGlobalToGround(collider_pos);
		//衝突判定
		if (planet_component_->GetHeightByRad(collider_ground_pos.x) + collider.radius()*collider.GetCollisionScale() > collider_ground_pos.y) {
			auto& collider_transform = collider.transform2d();
			//速度の修正
			collider_transform.ground_velocity({ collider_transform.ground_velocity().x, 0 });
			//押し出し
			collider_transform.ground_position({ collider_transform.ground_position().x, planet_component_->GetHeightByRad(collider_transform.ground_position().x) + collider.radius()*collider.GetCollisionScale() });
			return true;
		} else {
			return false;
		}
	}

	double CPlanetGround2D::GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos) const {
		return ground_pos.y - planet_component_->GetHeightByRad(ground_pos.x);
	}

	Vector2Dd CPlanetGround2D::ConvertPositionGlobalToGround(const Vector2Dd& global_pos) const {
		auto relative_pos = global_pos - transform2d_->position();
		return Vector2Dd(-std::atan2(relative_pos.y, relative_pos.x), relative_pos.length());
	}

	Vector2Dd CPlanetGround2D::ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos) const {
		return Vector2Dd(std::cos(-ground_pos.x), std::sin(-ground_pos.x))*ground_pos.y + transform2d_->position();
	}

	Vector2Dd CPlanetGround2D::NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector) const {
		return Vector2Dd(ground_pos.y == 0 ? 0 : ground_vector.x / ground_pos.y, ground_vector.y);
	}

	double CPlanetGround2D::GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos) const {
		return ground_pos.x + math::PI / 2;
	}
}