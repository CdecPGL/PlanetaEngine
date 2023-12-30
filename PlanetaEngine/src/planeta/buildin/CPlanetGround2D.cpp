#include "..\core\i_game_object.hpp"
#include "..\core\log_utility.hpp"
#include "..\math\math_constant.hpp"

#include "CPlanetGround2D.hpp"
#include "CTransform2D.hpp"
#include "CCircleCollider2D.hpp"
#include "CPlanet.hpp"

namespace plnt {
	bool CPlanetGround2D::get_other_components_proc(const go_component_getter &com_getter) {
		if (!CGround2D::get_other_components_proc(com_getter)) { return false; };
		planet_component_.reset(com_getter.get_component<CPlanet>());
		if (!planet_component_) {
			PE_LOG_ERROR("PlanetComponentを取得できませんでした。");
			return false;
		} else { return true; }
	}

	bool CPlanetGround2D::collide_with(CCircleCollider2D &collider) {
		//			TransformComponent& transform = game_object().transform();
		auto collider_pos = collider.GetCollisionGlobalCenterPosition();
		//コライダーの惑星座標を求める
		auto collider_ground_pos = ConvertPositionGlobalToGround(collider_pos);
		//衝突判定
		if (planet_component_->GetHeightByRad(collider_ground_pos.x) + collider.radius() * collider.GetCollisionScale()
			> collider_ground_pos.y) {
			auto &collider_transform = collider.transform2d();
			//速度の修正
			collider_transform.ground_velocity({collider_transform.ground_velocity().x, 0});
			//押し出し
			collider_transform.ground_position({
				collider_transform.ground_position().x,
				planet_component_->GetHeightByRad(collider_transform.ground_position().x) + collider.radius() * collider
				.GetCollisionScale()
			});
			return true;
		} else { return false; }
	}

	double CPlanetGround2D::GetAltitudeWithGroundPosition(const vector_2dd &ground_pos) const {
		return ground_pos.y - planet_component_->GetHeightByRad(ground_pos.x);
	}

	vector_2dd CPlanetGround2D::ConvertPositionGlobalToGround(const vector_2dd &global_pos) const {
		auto relative_pos = global_pos - transform2d().position();
		return vector_2dd(-std::atan2(relative_pos.y, relative_pos.x), relative_pos.length());
	}

	vector_2dd CPlanetGround2D::ConvertPositionGroundToGlobal(const vector_2dd &ground_pos) const {
		return vector_2dd(std::cos(-ground_pos.x), std::sin(-ground_pos.x)) * ground_pos.y + transform2d().position();
	}

	vector_2dd CPlanetGround2D::NormalizeGroundVectorWithGroundPosition(const vector_2dd &ground_pos,
	                                                                   const vector_2dd &ground_vector) const {
		return vector_2dd(ground_pos.y == 0 ? 0 : ground_vector.x / ground_pos.y, ground_vector.y);
	}

	double CPlanetGround2D::GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(
		const vector_2dd &ground_pos) const { return ground_pos.x + math::pi / 2; }
}
