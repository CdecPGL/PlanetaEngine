#pragma once

#include "CGround2D.hpp"

namespace plnt {
	class CPlanet;

	/*! 惑星地面コンポーネント*/
	class CPlanetGround2D final : public CGround2D {
	public:
		using Super = CPlanetGround2D;

	private:
		NonOwingPointer<CPlanet> planet_component_;
		bool GetOtherComponentsProc(const GOComponentGetter &com_getter) override;
		virtual bool CollideWith(CCircleCollider2D &circle_collider) override;
		double GetAltitudeWithGroundPosition(const Vector2Dd &ground_pos) const override;
		Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd &global_pos) const override;
		Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd &ground_pos) const override;
		Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd &ground_pos,
		                                                  const Vector2Dd &ground_vector) const override;
		double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd &ground_pos) const override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CPlanetGround2D);
}
