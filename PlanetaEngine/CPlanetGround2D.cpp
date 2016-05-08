#include "CPlanetGround2D.h"
#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "CCircleCollider2D.h"
#include "CPlanet.h"
#include "SystemLog.h"
#include "MathConstant.h"

namespace planeta_engine {
	bool CPlanetGround2D::OnInitialized() {
		if (CGround2D::OnInitialized() == false) {
			debug::SystemLog::instance().LogError("GroundComponentの初期化に失敗しました。", "PlanetGroundComponent::Initialize_");
			return false;
		} else {
			planet_component_.reset(game_object().GetComponent<CPlanet>());
			if (!planet_component_) {
				debug::SystemLog::instance().LogError("PlanetComponentを取得できませんでした。", "PlanetGroundComponent::Initialize_");
				return false;
			} else {
				return true;
			}
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
			collider_transform.velocity(Vector2Dd(collider_transform.velocity().x, 0));
			//押し出し
			collider_transform.position(Vector2Dd(collider_transform.position().x, planet_component_->GetHeightByRad(collider_transform.position().x) + collider.radius()*collider.GetCollisionScale()));
			return true;
		} else {
			return false;
		}
	}

	double CPlanetGround2D::GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos) const {
		return ground_pos.y - planet_component_->GetHeightByRad(ground_pos.x);
	}

	Vector2Dd CPlanetGround2D::ConvertPositionGlobalToGround(const Vector2Dd& global_pos) const {
		auto relative_pos = global_pos - transform2d_->global_position();
		return Vector2Dd(-std::atan2(relative_pos.y, relative_pos.x), relative_pos.length());
	}

	Vector2Dd CPlanetGround2D::ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos) const {
		return Vector2Dd(std::cos(-ground_pos.x), std::sin(-ground_pos.x))*ground_pos.y + transform2d_->global_position();
	}

	Vector2Dd CPlanetGround2D::NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector) const {
		return Vector2Dd(ground_pos.y == 0 ? 0 : ground_vector.x / ground_pos.y, ground_vector.y);
	}

	double CPlanetGround2D::GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos) const {
		return ground_pos.x + math::PI / 2;
	}
}