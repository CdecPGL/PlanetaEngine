#pragma once

#include "WeakPointer.hpp"

namespace planeta {
	class IGameObject;
	using GameObjectAccessorType = WeakPointer<IGameObject>;
	template<class CT>
	using GOComponentAccessorType = WeakPointer<CT>;
}