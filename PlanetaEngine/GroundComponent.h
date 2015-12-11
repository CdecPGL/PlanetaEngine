#pragma once
#include "GameObjectNormalComponent.h"
#include "Matrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
		class GroundComponent :
			public GameObjectNormalComponent , public core::IColliderWithCollider
		{
		public:
			GroundComponent();
			virtual ~GroundComponent() = default;
			virtual const Vector2D<double> GetDownwardDirectionVector(const Vector2D<double>& pos)const = 0;
			virtual const double GetDownwardDirectionByRad(const Vector2D<double>& pos)const = 0;
			virtual const double GetAltitude(const Vector2D<double>& position)const = 0;
			Vector2D<double> ConvertGlobalVectorToLocalVector(const Vector2D<double>& position, const Vector2D<double>& vector)const { return math::RotationalTransformation(-(GetDownwardDirectionByRad(position) + math::constant::PI / 2.0), vector); }
			Vector2D<double> ConvertLocalVectorToGlobalVector(const Vector2D<double>& position, const Vector2D<double>& vector)const { return math::RotationalTransformation((GetDownwardDirectionByRad(position) + math::constant::PI / 2.0), vector); }
		protected:
			static Vector2D<double> ConvertRadToVec(double rad){ return Vector2D<double>(std::cos(rad), std::sin(rad)); }
			static double ConvertVecToRad(const Vector2D<double>& vec) { return std::atan2(vec.y, vec.x); }

			virtual bool OnInitialized() override;
			virtual bool OnActivated() override;
			virtual void OnUpdated() override;
			virtual bool OnInactivated() override;
			virtual void OnFinalized() throw() override;
		};
	}
}
