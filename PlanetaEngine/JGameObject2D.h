﻿#pragma once

#include "GameObjectInterface.h"
#include "CTransform2D.h"

namespace planeta {
	class JGameObject2D : public GameObjectInterface<JGameObject2D>{
	public:
		using GameObjectInterface::GameObjectInterface;
		virtual CTransform2D& transform2d() = 0;
		virtual const CTransform2D& transform2d()const = 0;
	};
}
