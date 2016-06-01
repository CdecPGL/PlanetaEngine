#pragma once
#include "CCollider2D.h"

namespace planeta {
	/*‘½ŠpŒ`Õ“ËƒNƒ‰ƒX(–¢À‘•)*/
	class CPolygonCollider2D : public CCollider2D {
		bool DetectCollision(core::IColliderWithCollider2D& collider)override { return collider.CollideWith(*this); }
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CPolygonCollider2D);
}
