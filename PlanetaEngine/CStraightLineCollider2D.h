#pragma once

#include "CCollider2D.h"

namespace planeta {
	class CStraightLineCollider2D final : public CCollider2D {
	public:
		using Super = CCollider2D;
		bool DetectCollision(core::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
		const double length()const { return length_; }
		CStraightLineCollider2D& length(double l) { length_ = l; return *this; }
	private:
		virtual bool CollideWith(CCircleCollider2D& circle_collider) override;

		double length_ = 1.0;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CStraightLineCollider2D);
}
