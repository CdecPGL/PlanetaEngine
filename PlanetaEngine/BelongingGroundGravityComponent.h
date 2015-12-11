#pragma once
#include "GameObjectComponent.h"

namespace planeta_engine {
	namespace components {
		class BelongingGroundGravityComponent : public game::GameObjectComponent {
		public:
			const double gravity_scale()const { return _gravity_scale; }
			void gravity_scale(double gs) { _gravity_scale = gs; }
		private:
			double _gravity_scale = 0.5;
			bool Initialize_() override;
			void Update_() override;
		};
	}
}