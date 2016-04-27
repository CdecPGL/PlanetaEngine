#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	using GameObjectAccessorType = utility::WeakPointer<IGameObject>;
	template<class CT>
	using GOComponentAccessorType = utility::WeakPointer<CT>;
}