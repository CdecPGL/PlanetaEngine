#pragma once

#include "CCollider2D.hpp"

namespace plnt {
	/*! 円形コライダーコンポーネント*/
	class CCircleCollider2D final : public CCollider2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CCircleCollider2D);

	public:
		using Super = CCollider2D;

		bool DetectCollision(private_::IColliderWithCollider2D &collider) override {
			return collider.CollideWith(*this);
		}

		/*! 半径を取得*/
		const double radius() const { return _radius; }
		/*! 半径を設定*/
		CCircleCollider2D &radius(double r) {
			_radius = r;
			return *this;
		}

	private:
		virtual bool CollideWith(CCircleCollider2D &circle_collider) override;
		virtual bool CollideWith(CStraightLineCollider2D &straight_line_collider) override;


		double _radius = 1.0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CCircleCollider2D);
}
