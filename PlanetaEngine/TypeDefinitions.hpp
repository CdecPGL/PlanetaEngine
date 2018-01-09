#pragma once

#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	using GameObjectAccessorType = WeakPointer<IGameObject>;
	template<class CT>
	using GOComponentAccessorType = WeakPointer<CT>;
}