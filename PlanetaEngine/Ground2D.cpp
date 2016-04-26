#include "CGround2D.h"
#include "Matrix2_2.h"

namespace planeta_engine {
	CGround2D::CGround2D() {}

	bool CGround2D::OnInitialized() {
		return true;
	}

	bool CGround2D::OnActivated() {
		return true;
	}

	void CGround2D::OnUpdated() {

	}

	bool CGround2D::OnInactivated() {
		return true;
	}

	void CGround2D::OnFinalized() noexcept {

	}

	double CGround2D::ConvertRotationGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, double global_rota_rad) const {
		return global_rota_rad + GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
	}

	double CGround2D::ConvertRotationGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, double ground_rota_rad) const {
		return ground_rota_rad - GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
	}

	Vector2Dd CGround2D::ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& global_velocity) {
		return math::RotationalTransformation(GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos), global_velocity);
	}

	Vector2Dd CGround2D::ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_velocity) {
		return math::RotationalTransformation(-GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos), ground_velocity);
	}
}
