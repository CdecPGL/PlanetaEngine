#pragma once

#include "CCollider2D.hpp"

namespace plnt {
	/*! 直線コライダーコンポーネント*/
	class c_straight_line_collider_2d final : public c_collider_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_straight_line_collider_2d);

	public:
		using super = c_collider_2d;

		bool detect_collision(i_collider_with_collider_2d &collider) override {
			return collider.collide_with(*this);
		}

		/*! 長さを取得*/
		[[nodiscard]] double length() const { return length_; }
		/*! 長さを設定*/
		c_straight_line_collider_2d &length(const double l) {
			length_ = l;
			return *this;
		}

	private:
		bool collide_with(c_circle_collider_2d &circle_collider) override;

		double length_ = 1.0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_straight_line_collider_2d);
}
