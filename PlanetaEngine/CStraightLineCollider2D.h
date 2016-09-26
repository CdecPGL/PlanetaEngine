#pragma once

#include "CCollider2D.h"

namespace planeta {
	/*! 直線コライダーコンポーネント*/
	class CStraightLineCollider2D final : public CCollider2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CStraightLineCollider2D);
	public:
		using Super = CCollider2D;
		bool DetectCollision(private_::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
		/*! 長さを取得*/
		const double length()const { return length_; }
		/*! 長さを設定*/
		CStraightLineCollider2D& length(double l) { length_ = l; return *this; }
	private:
		virtual bool CollideWith(CCircleCollider2D& circle_collider) override;

		double length_ = 1.0;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CStraightLineCollider2D);
}
