#pragma once
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {
		class PlanetComponent;
		class PlanetGroundComponent final: public GroundComponent {
		public:
			const Vector2D<double> GetDownwardDirectionVector(const Vector2D<double>& pos)const override;
			const double GetDownwardDirectionByRad(const Vector2D<double>& pos)const override { return ConvertVecToRad(GetDownwardDirectionVector(pos)); }
			const double GetAltitude(const Vector2D<double>& position)const override;
		private:
			utility::WeakPointer<PlanetComponent> planet_component_;
			bool Initialize_()override;
			virtual bool CollideWith(components::CircleColliderComponent& circle_collider) override;

		};
	}
}

