#include "planeta/buildin/CGround2D.hpp"
#include "planeta/Matrix2_2.hpp"
#include "planeta/IGameObject.hpp"
#include "planeta/LogUtility.hpp"
#include "planeta/buildin/CTransform2D.hpp"

namespace plnt {
	CGround2D::CGround2D() {}

	bool CGround2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void CGround2D::OnActivated() {
		Super::OnActivated();
	}

	void CGround2D::OnInactivated() {
		Super::OnInactivated();
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

	const plnt::CTransform2D& CGround2D::transform2d() const {
		return *transform2d_;
	}

}
