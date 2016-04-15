#pragma once
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {
		class PlanetComponent;
		class PlanetGroundComponent final: public GroundComponent {
		public:
			
		private:
			utility::WeakPointer<PlanetComponent> planet_component_;
			bool OnInitialized()override;
			virtual bool CollideWith(components::CircleColliderComponent& circle_collider) override;
			double GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos) const override;
			Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd& global_pos) const override;
			Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos) const override;
			Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector) const override;
			double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos) const override;
		};
	}
}

