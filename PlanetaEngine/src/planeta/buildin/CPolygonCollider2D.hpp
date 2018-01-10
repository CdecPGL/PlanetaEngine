#pragma once
#include "planeta/buildin/CCollider2D.hpp"

namespace plnt {
	/*! 多角形コライダーコンポーネント(未実装)*/
	class CPolygonCollider2D : public CCollider2D {
	public:
		using Super = CCollider2D;
	private:
		bool DetectCollision(private_::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CPolygonCollider2D);
}
