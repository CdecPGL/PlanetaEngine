#pragma once

#include "CCollider2D.h"

namespace planeta_engine {
	class CCircleCollider2D final : public CCollider2D {
	public:
		bool DetectCollision(core::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
		const double radius()const { return _radius; }
		CCircleCollider2D& radius(double r) { _radius = r; return *this; }
	private:
		virtual bool CollideWith(CCircleCollider2D& circle_collider) override;
		virtual bool CollideWith(CStraightLineCollider2D& straight_line_collider) override;


		double _radius = 1.0;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CCircleCollider2D);
}
