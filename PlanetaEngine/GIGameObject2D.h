#pragma once

#include "GameObjectInterface.h"
#include "CTransform2D.h"

namespace planeta_engine {
	class GIGameObject2D : public GameObjectInterface{
	public:
		virtual CTransform2D& transform2d() = 0;
		const CTransform2D& transform2d()const { return const_cast<GIGameObject2D*>(this)->transform2d(); }
	};
}
