#pragma once

#include "GameObjectComponent.h"

namespace planeta_engine {
	class GameObjectNormalComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectNormalComponent() = default;
	private:
		/*����ݒ�͍s��Ȃ�*/
		bool SpecialSetUp(const core::SceneDataForGameObject&)override final{ return true; }
	};
}