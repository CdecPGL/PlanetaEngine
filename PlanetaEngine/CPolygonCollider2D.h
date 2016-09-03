#pragma once
#include "CCollider2D.h"

namespace planeta {
	/*多角形衝突クラス(未実装)*/
	class CPolygonCollider2D : public CCollider2D {
	public:
		using Super = CCollider2D;
	private:
		bool DetectCollision(core::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CPolygonCollider2D);
}
