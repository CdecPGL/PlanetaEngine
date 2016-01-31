#pragma once
#include "GameObjectNormalComponent.h"

namespace planeta_engine {
	namespace components {
		class BelongingGroundGravityComponent : public GameObjectNormalComponent {
		public:
			const double gravity_scale()const { return _gravity_scale; }
			BelongingGroundGravityComponent& gravity_scale(double gs) { _gravity_scale = gs; return *this; }
		private:
			double _gravity_scale = 0.5;
			bool OnInitialized() override;
			void OnUpdated() override;
		};
	}
}