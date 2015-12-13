#pragma once

#include "GameObjectComponent.h"

namespace planeta_engine {
	class GameObjectNormalComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectNormalComponent() = default;
	private:
		/*特殊設定は行わない*/
		bool SpecialSetUp(const core::SceneDataForGameObject&)override final{ return true; }
	};
}