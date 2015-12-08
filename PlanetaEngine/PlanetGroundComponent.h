#pragma once
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {
		class PlanetComponent;
		class PlanetGroundComponent final: public GroundComponent {
		public:
			const math::Vector2Dd GetDownwardDirectionVector(const math::Vector2Dd& pos)const override;
			const double GetDownwardDirectionByRad(const math::Vector2Dd& pos)const override { return ConvertVecToRad(GetDownwardDirectionVector(pos)); }
			const double GetAltitude(const math::Vector2Dd& position)const override;
		private:
			utility::WeakPointer<PlanetComponent> planet_component_;
			bool Initialize_()override;
			virtual bool CollideWith(components::CircleColliderComponent& circle_collider) override;

		};
	}
}

