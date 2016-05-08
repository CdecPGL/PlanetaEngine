#include "CTransform2D.h"
#include "Transform2DCore.h"
#include <cassert>
#include "IGameObject.h"
#include "SystemLog.h"
#include "CGround2D.h"

#include "RootTransform2DCore.h"

namespace planeta_engine {
	CTransform2D::CTransform2D() :core_(std::make_unique<RootTransform2DCore>()) {};

	CTransform2D::~CTransform2D() = default;

	void CTransform2D::Move(const Vector2Dd& mov_pos) {
		Offset(position(), mov_pos);
	}

	void CTransform2D::Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) {
		core_->Offset(base_pos, offset);
	}

	void CTransform2D::Accelerate(const Vector2Dd& acceleration) {
		velocity(velocity() + acceleration);
	}

	void CTransform2D::GlobalAccelerate(const Vector2Dd& global_acceleration) {
		global_velocity(global_velocity() + global_acceleration);
	}

	void CTransform2D::ApplyVelocity() {
		Move(velocity());
		rotation_rad(rotation_rad() + rotation_velocity_rad());
	}

	const Vector2Dd& CTransform2D::position() const {
		return core_->position();
	}

	CTransform2D&  CTransform2D::position(const Vector2Dd& pos) {
		core_->position(pos);
		return *this;
	}

	const Vector2Dd& CTransform2D::scale() const {
		return core_->scale();
	}

	CTransform2D&  CTransform2D::scale(const Vector2Dd& s) {
		core_->scale(s);
		return *this;
	}

	const double CTransform2D::rotation_rad() const {
		return core_->rotation_rad();
	}

	CTransform2D&  CTransform2D::rotation_rad(double rota_rad) {
		core_->rotation_rad(rota_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::global_position() const {
		return core_->global_position();
	}

	CTransform2D&  CTransform2D::global_position(const Vector2Dd& pos) {
		core_->global_position(pos);
		return *this;
	}

	const double CTransform2D::global_rotation_rad() const {
		return core_->global_rotation_rad();
	}

	CTransform2D&  CTransform2D::global_rotation_rad(double rota_rad) {
		core_->global_rotation_rad(rota_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::velocity() const {
		return core_->velocity();
	}

	CTransform2D&  CTransform2D::velocity(const Vector2Dd& vel) {
		core_->velocity(vel);
		return *this;
	}

	const double CTransform2D::rotation_velocity_rad() const {
		return core_->rotation_velocity_rad();
	}

	CTransform2D&  CTransform2D::rotation_velocity_rad(double rota_vel_rad) {
		core_->rotation_velocity_rad(rota_vel_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::global_velocity()const {
		return core_->global_velocity();
	}

	CTransform2D&  CTransform2D::global_velocity(const Vector2Dd& vel) {
		core_->global_velocity(vel);
		return *this;
	}

	const CGround2D& CTransform2D::ground() const {
		return const_cast<CTransform2D*>(this)->ground();
	}

	CGround2D& CTransform2D::ground() {
		return core_->ground();
	}

	utility::WeakPointer<CGround2D> CTransform2D::GetGround() const {
		return core_->GetGround();
	}

	bool CTransform2D::SetGround(const utility::WeakPointer<IGameObject>& g, bool keep_global_position) {
		auto gcom = g->GetComponent<CGround2D>();
		if (gcom) {
			core_->SetGround(gcom, keep_global_position);
			return true;
		} else {
			PE_LOG_ERROR("Groundコンポーネントを持たないGameObjectがTransformのGroundとして渡されました。");
			return false;
		}
	}
}