#include "PlanetGroundComponent.h"
#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "CircleColliderComponent.h"
#include "PlanetComponent.h"
#include "SystemLog.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {
		bool PlanetGroundComponent::OnInitialized()
		{
			if (GroundComponent::OnInitialized() == false) {
				debug::SystemLog::instance().LogError("GroundComponentの初期化に失敗しました。", "PlanetGroundComponent::Initialize_");
				return false;
			}
			else {
				planet_component_ = game_object().GetComponent<PlanetComponent>();
				if (!planet_component_) {
					debug::SystemLog::instance().LogError("PlanetComponentを取得できませんでした。", "PlanetGroundComponent::Initialize_");
					return false;
				}
				else {
					return true;
				}
			}
		}

		bool PlanetGroundComponent::CollideWith(components::CircleColliderComponent& collider) {
//			TransformComponent& transform = game_object().transform();
			auto collider_pos = collider.GetCollisionGlobalCenterPosition();
			//コライダーの惑星座標を求める
			auto collider_ground_pos = ConvertPositionGlobalToGround(collider_pos);
			//衝突判定
			if (planet_component_->GetHeightByRad(collider_ground_pos.x) + collider.radius()*collider.GetCollisionScale() > collider_ground_pos.y) {
				auto& collider_transform = collider.game_object().transform();
				//速度の修正
				collider_transform.velocity(Vector2D<double>(collider_transform.velocity().x, 0));
				//押し出し
				collider_transform.position(Vector2D<double>(collider_transform.position().x, planet_component_->GetHeightByRad(collider_transform.position().x) + collider.radius()*collider.GetCollisionScale()));
				return true;
			} else {
				return false;
			}
		}

		double PlanetGroundComponent::GetAltitudeWithGroundPosition(const Vector2D<double>& ground_pos) const {
			return ground_pos.y - planet_component_->GetHeightByRad(ground_pos.x);
		}

		Vector2D<double> PlanetGroundComponent::ConvertPositionGlobalToGround(const Vector2D<double>& global_pos) const {
			auto relative_pos = global_pos - game_object().transform().global_position();
			return Vector2D<double>(-std::atan2(relative_pos.y, relative_pos.x), relative_pos.Length());
		}

		Vector2D<double> PlanetGroundComponent::ConvertPositionGroundToGlobal(const Vector2D<double>& ground_pos) const {
			return Vector2D<double>(std::cos(-ground_pos.x), std::sin(-ground_pos.x))*ground_pos.y + game_object().transform().global_position();
		}

		Vector2D<double> PlanetGroundComponent::NormalizeGroundVectorWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_vector) const {
			return Vector2D<double>(ground_vector.x / ground_pos.y, ground_vector.y);
		}

		double PlanetGroundComponent::GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2D<double>& ground_pos) const {
			return ground_pos.x + math::constant::PI / 2;
		}

	}
}