#pragma once

#include "NonCopyable.h"

namespace planeta_engine{
	namespace game{
		class GameObjectAccessorForSetUp;
		class GameObject;
		class GameObjectSetUpper : private utility::NonCopyable<GameObjectSetUpper>{
		public:
			GameObjectSetUpper() = default;
			virtual ~GameObjectSetUpper() = default;
			bool operator()(GameObject& game_object);
		private:
			virtual bool _SetUpGameObject(GameObjectAccessorForSetUp& gosup) = 0;
		};
	}
}