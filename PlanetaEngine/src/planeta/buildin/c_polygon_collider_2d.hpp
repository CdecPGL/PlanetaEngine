#pragma once

#include "c_collider_2d.hpp"

namespace plnt {
	/*! 多角形コライダーコンポーネント(未実装)*/
	class c_polygon_collider_2d final : public c_collider_2d {
	public:
		using super = c_collider_2d;

	private:
		bool detect_collision(i_collider_with_collider_2d &collider) override {
			return collider.collide_with(*this);
		}
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_polygon_collider_2d);
}
