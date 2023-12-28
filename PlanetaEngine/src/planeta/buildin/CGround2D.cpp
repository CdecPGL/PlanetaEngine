#include "planeta/core/Matrix2_2.hpp"
#include "planeta/core/IGameObject.hpp"
#include "planeta/core/LogUtility.hpp"

#include "CGround2D.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	CGround2D::CGround2D() { }

	bool CGround2D::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void CGround2D::on_activated() { super::on_activated(); }

	void CGround2D::on_inactivated() { super::on_inactivated(); }

	void CGround2D::on_finalized() noexcept { }

	double CGround2D::ConvertRotationGlobalToGroundWithGroundPosition(const Vector2Dd &ground_pos,
	                                                                  double global_rota_rad) const {
		return global_rota_rad + GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
	}

	double CGround2D::ConvertRotationGroundToGlobalWithGroundPosition(const Vector2Dd &ground_pos,
	                                                                  double ground_rota_rad) const {
		return ground_rota_rad - GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos);
	}

	Vector2Dd CGround2D::ConvertVelocityGlobalToGroundWithGroundPosition(
		const Vector2Dd &ground_pos, const Vector2Dd &global_velocity) {
		return math::RotationalTransformation(GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos),
		                                      global_velocity);
	}

	Vector2Dd CGround2D::ConvertVelocityGroundToGlobalWithGroundPosition(
		const Vector2Dd &ground_pos, const Vector2Dd &ground_velocity) {
		return math::RotationalTransformation(-GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(ground_pos),
		                                      ground_velocity);
	}

	CTransform2D &CGround2D::transform2d() { return *transform2d_; }

	const plnt::CTransform2D &CGround2D::transform2d() const { return *transform2d_; }
}
