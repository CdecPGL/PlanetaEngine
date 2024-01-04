#pragma once

#include "c_collider_2d.hpp"

namespace plnt {
	/*! 円形コライダーコンポーネント*/
	class c_circle_collider_2d final : public c_collider_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_circle_collider_2d);

	public:
		using super = c_collider_2d;

		bool detect_collision(i_collider_with_collider_2d &collider) override {
			return collider.collide_with(*this);
		}

		/*! 半径を取得*/
		[[nodiscard]] double radius() const { return radius_; }
		/*! 半径を設定*/
		[[nodiscard]] c_circle_collider_2d &radius(const double r) {
			radius_ = r;
			return *this;
		}

	private:
		bool collide_with(c_circle_collider_2d &circle_collider) override;
		bool collide_with(c_straight_line_collider_2d &straight_line_collider) override;


		double radius_ = 1.0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_circle_collider_2d);
}
