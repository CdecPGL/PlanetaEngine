#include "CGround2D.h"
#include "Matrix2_2.h"
#include "IGameObjectForComponent.h"
#include "SystemLog.h"
#include "CTransform2D.h"

namespace planeta {
	CGround2D::CGround2D() {}

	bool CGround2D::GetOtherComponentProc(const GOComponentGetter& com_getter) {
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2DÇ™éÊìæÇ≈Ç´Ç‹ÇπÇÒÇ≈ÇµÇΩÅB");
			return false;
		}
		return true;
	}

	bool CGround2D::OnInitialized() {
		return true;
	}

	bool CGround2D::OnActivated() {
		return true;
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

	CTransform2D& CGround2D::transform2d() {
		return *transform2d_;
	}

}
