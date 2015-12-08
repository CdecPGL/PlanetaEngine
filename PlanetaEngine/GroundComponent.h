#pragma once
#include "Component.h"
#include "Matrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
		class GroundComponent :
			public game::Component , public core::IColliderWithCollider
		{
		public:
			GroundComponent();
			virtual ~GroundComponent() = default;
			virtual const math::Vector2Dd GetDownwardDirectionVector(const math::Vector2Dd& pos)const = 0;
			virtual const double GetDownwardDirectionByRad(const math::Vector2Dd& pos)const = 0;
			virtual const double GetAltitude(const math::Vector2Dd& position)const = 0;
			math::Vector2Dd ConvertGlobalVectorToLocalVector(const math::Vector2Dd& position, const math::Vector2Dd& vector)const { return math::RotationalTransformation(-(GetDownwardDirectionByRad(position) + math::constant::PI / 2.0), vector); }
			math::Vector2Dd ConvertLocalVectorToGlobalVector(const math::Vector2Dd& position, const math::Vector2Dd& vector)const { return math::RotationalTransformation((GetDownwardDirectionByRad(position) + math::constant::PI / 2.0), vector); }
		protected:
			static math::Vector2Dd ConvertRadToVec(double rad){ return math::Vector2Dd(std::cos(rad), std::sin(rad)); }
			static double ConvertVecToRad(const math::Vector2Dd& vec) { return std::atan2(vec.y, vec.x); }

			virtual bool Initialize_() override;
			virtual bool Activated_() override;
			virtual void Update_() override;
			virtual bool InActivated_() override;
			virtual void Finalize_() throw() override;
		};
	}
}
