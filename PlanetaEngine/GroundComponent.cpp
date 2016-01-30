#include "GroundComponent.h"
#include "Matrix.h"

namespace planeta_engine{
	namespace components {
		GroundComponent::GroundComponent()
		{
		}

		bool GroundComponent::OnInitialized()
		{
			return true;
		}

		bool GroundComponent::OnActivated()
		{
			return true;
		}

		void GroundComponent::OnUpdated()
		{

		}

		bool GroundComponent::OnInactivated()
		{
			return true;
		}

		void GroundComponent::OnFinalized() noexcept
		{

		}

		double GroundComponent::ConvertRotationGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, double global_rota_rad) const {
			return global_rota_rad + GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
		}

		double GroundComponent::ConvertRotationGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, double ground_rota_rad) const {
			return ground_rota_rad - GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
		}

		Vector2D<double> GroundComponent::ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& global_velocity) {
			return math::RotationalTransformation(GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos), global_velocity);
		}

		Vector2D<double> GroundComponent::ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_velocity) {
			return math::RotationalTransformation(-GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos), ground_velocity);
		}

	}
}
