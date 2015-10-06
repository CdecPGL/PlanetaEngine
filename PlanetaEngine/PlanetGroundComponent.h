#pragma once
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {
		class PlanetComponent;
		class PlanetGroundComponent : public GroundComponent {
		public:
			const Vector2D<double> GetDownwardDirectionVector(const Vector2D<double>& pos)const override;
			const double GetDownwardDirectionByRad(const Vector2D<double>& pos)const override { return ConvertVecToRad(GetDownwardDirectionVector(pos)); }
			const double GetAltitude(const Vector2D<double>& position)const override;

			bool CollisionDetect(CircleColliderComponent& collider) override;
			bool CollisionDetect(StraightLineColliderComponent& collider) override;
		private:
			utility::WeakPointer<PlanetComponent> planet_component_;
			bool Initialize_()override;
		};
	}
}

