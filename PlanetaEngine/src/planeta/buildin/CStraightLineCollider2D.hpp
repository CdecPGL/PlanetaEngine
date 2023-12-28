#pragma once

#include "CCollider2D.hpp"

namespace plnt {
	/*! 直線コライダーコンポーネント*/
	class CStraightLineCollider2D final : public CCollider2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CStraightLineCollider2D);

	public:
		using super = CCollider2D;

		bool DetectCollision(private_::i_collider_with_collider_2d &collider) override {
			return collider.collide_with(*this);
		}

		/*! 長さを取得*/
		const double length() const { return length_; }
		/*! 長さを設定*/
		CStraightLineCollider2D &length(double l) {
			length_ = l;
			return *this;
		}

	private:
		virtual bool collide_with(CCircleCollider2D &circle_collider) override;

		double length_ = 1.0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CStraightLineCollider2D);
}
