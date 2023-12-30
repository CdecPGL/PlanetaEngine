#pragma once

#include "CGround2D.hpp"

namespace plnt {
	class CPlanet;

	/*! 惑星地面コンポーネント*/
	class CPlanetGround2D final : public CGround2D {
	public:
		using super = CPlanetGround2D;

	private:
		non_owing_pointer<CPlanet> planet_component_;
		bool get_other_components_proc(const go_component_getter &com_getter) override;
		virtual bool collide_with(CCircleCollider2D &circle_collider) override;
		double GetAltitudeWithGroundPosition(const vector_2dd &ground_pos) const override;
		vector_2dd ConvertPositionGlobalToGround(const vector_2dd &global_pos) const override;
		vector_2dd ConvertPositionGroundToGlobal(const vector_2dd &ground_pos) const override;
		vector_2dd NormalizeGroundVectorWithGroundPosition(const vector_2dd &ground_pos,
		                                                  const vector_2dd &ground_vector) const override;
		double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const vector_2dd &ground_pos) const override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CPlanetGround2D);
}
