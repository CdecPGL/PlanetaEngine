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
			double GetAltitudeWithGroundPosition(const Vector2D<double>& ground_pos) const override;
			Vector2D<double> ConvertPositionGlobalToGround(const Vector2D<double>& global_pos) const override;
			Vector2D<double> ConvertPositionGroundToGlobal(const Vector2D<double>& ground_pos) const override;
			Vector2D<double> NormalizeGroundVectorWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_vector) const override;
			double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2D<double>& ground_pos) const override;
		};
	}
}

