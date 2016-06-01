#pragma once

#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	using GameObjectAccessorType = util::WeakPointer<IGameObject>;
	template<class CT>
	using GOComponentAccessorType = util::WeakPointer<CT>;
}