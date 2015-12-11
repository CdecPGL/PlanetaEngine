#pragma once

#include "GameObjectComponent.h"

namespace planeta_engine {
	class GameObjectNormalComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectNormalComponent() = default;
	private:
		/*ì¡éÍê›íËÇÕçsÇÌÇ»Ç¢*/
		bool SpecialSetUp(const core::GameObjectComponentSpecialSetUpData&)override final{ return true; }
	};
}