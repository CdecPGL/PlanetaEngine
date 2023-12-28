#pragma once

#include "CCollider2D.hpp"

namespace plnt {
	/*! 多角形コライダーコンポーネント(未実装)*/
	class CPolygonCollider2D : public CCollider2D {
	public:
		using super = CCollider2D;

	private:
		bool DetectCollision(private_::i_collider_with_collider_2d &collider) override {
			return collider.collide_with(*this);
		}
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CPolygonCollider2D);
}
