#include "CTransform2D.h"
#include "Transform2DCore.h"
#include <cassert>
#include "IGameObject.h"
#include "SystemLog.h"
#include "CGround2D.h"

#include "RootTransform2DCore.h"

namespace planeta_engine {
	//////////////////////////////////////////////////////////////////////////
	//Impl
	//////////////////////////////////////////////////////////////////////////
	class CTransform2D::Impl_ {
	public:
		Impl_::Impl_():core(std::make_unique<RootTransform2DCore>()){}
		//バッファデータ(適用前データ)
		Transform2DCore::AllData local_buffer;
		Transform2DCore::AllData global_buffer;
		Transform2DCore::UpdateState update_state; //更新状況
		//最終更新時のデータ
		Transform2DCore::AllData last_local;
		Transform2DCore::AllData last_global;

		std::unique_ptr<Transform2DCore> core;
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransform2D
	//////////////////////////////////////////////////////////////////////////
	CTransform2D::CTransform2D() :impl_(std::make_unique<Impl_>()) {};

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
		return impl_->last_local.transform.position;
	}

	CTransform2D&  CTransform2D::position(const Vector2Dd& pos) {
		impl_->local_buffer.transform.position = pos;
		return *this;
	}

	const Vector2Dd& CTransform2D::scale() const {
		return impl_->last_local.transform.scale;
	}

	CTransform2D&  CTransform2D::scale(const Vector2Dd& s) {
		impl_->local_buffer.transform.scale = s;
		return *this;
	}

	const double CTransform2D::rotation_rad() const {
		return impl_->last_local.transform.rotation_rad;
	}

	CTransform2D&  CTransform2D::rotation_rad(double rota_rad) {
		impl_->local_buffer.transform.rotation_rad = rota_rad;
		return *this;
	}

	const Vector2Dd& CTransform2D::global_position() const {
		return impl_->last_global.transform.position;
	}

	CTransform2D&  CTransform2D::global_position(const Vector2Dd& pos) {
		impl_->global_buffer.transform.position = pos;
		return *this;
	}

	const Vector2Dd& CTransform2D::global_scale() const {
		return impl_->last_global.transform.scale;
	}

	CTransform2D&  CTransform2D::global_scale(const Vector2Dd& s) {
		impl_->global_buffer.transform.scale = s;
		return *this;
	}

	const double CTransform2D::global_rotation_rad() const {
		return impl_->last_global.transform.rotation_rad;
	}

	CTransform2D&  CTransform2D::global_rotation_rad(double rota_rad) {
		impl_->global_buffer.transform.rotation_rad = rota_rad;
		return *this;
	}

	const Vector2Dd& CTransform2D::velocity() const {
		return impl_->last_local.phisical.velocity;
	}

	CTransform2D&  CTransform2D::velocity(const Vector2Dd& vel) {
		impl_->local_buffer.phisical.velocity = vel;
		return *this;
	}

	const double CTransform2D::rotation_velocity_rad() const {
		return impl_->last_local.phisical.rota_vel_rad;
	}

	CTransform2D&  CTransform2D::rotation_velocity_rad(double rota_vel_rad) {
		impl_->local_buffer.phisical.rota_vel_rad = rota_vel_rad;
		return *this;
	}

	const Vector2Dd& CTransform2D::global_velocity()const {
		return impl_->last_global.phisical.velocity;
	}

	CTransform2D&  CTransform2D::global_velocity(const Vector2Dd& vel) {
		impl_->global_buffer.phisical.velocity = vel;
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