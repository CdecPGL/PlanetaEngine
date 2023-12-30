#pragma once

#include <limits>

#include "..\math\math_constant.hpp"

#include "CGround2D.hpp"

namespace plnt {
	/*! 何もしない地面コンポーネント*/
	class CDumyGround2D final : public CGround2D {
	public:
		using super = CGround2D;

	private:
		double GetAltitudeWithGroundPosition(const vector_2dd &ground_pos) const override { return -1; }
		vector_2dd ConvertPositionGlobalToGround(const vector_2dd &global_pos) const override { return global_pos; }
		vector_2dd ConvertPositionGroundToGlobal(const vector_2dd &ground_pos) const override { return ground_pos; }

		vector_2dd NormalizeGroundVectorWithGroundPosition(const vector_2dd &ground_pos,
		                                                  const vector_2dd &ground_vector) const override {
			return ground_vector;
		}

		double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const vector_2dd &ground_pos) const override {
			return 0;
		}
	};

	//PE_GAMEOBJECTCOMPONENT_CLASS(CDumyGround2D);
}
