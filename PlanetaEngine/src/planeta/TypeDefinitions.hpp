#pragma once

#include "planeta/core/WeakPointer.hpp"

namespace plnt {
	class IGameObject;
	using GameObjectAccessorType = WeakPointer<IGameObject>;
	template<class CT>
	using GOComponentAccessorType = WeakPointer<CT>;
}