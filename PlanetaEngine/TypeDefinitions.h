#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
	}
	using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
	template<class CT>
	using GOComponentAccessorType = utility::WeakPointer<CT>;
}