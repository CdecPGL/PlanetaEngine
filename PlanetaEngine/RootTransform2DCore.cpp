#include <cassert>

#include "RootTransform2DCore.h"
#include "CGround2D.h"
#include "CDumyGround2D.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace {
		utility::WeakPointer<CDumyGround2D> GetDumyGround() {
			static std::shared_ptr<CDumyGround2D> com = std::make_shared<CDumyGround2D>();
			return com;
		}
	}

	RootTransform2DCore::RootTransform2DCore() :ground_(GetDumyGround()) {}

	void RootTransform2DCore::Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) {
		position(base_pos + ground().NormalizeGroundVectorWithGroundPosition(base_pos, offset));
	}

	void RootTransform2DCore::UpdateTransformDataGlobalByGround() {
		auto& ground = this->ground();
		if (position_last_update == CoordinateSystem::Ground) {
			global_transform_data_.position = ground.ConvertPositionGroundToGlobal(ground_transform_data_.position);
			//位置に応じて回転度が変わる可能性があるので、回転度の最終更新がGlobalでなかったら回転度も更新する。
			if (rotation_last_update != CoordinateSystem::Global) {
				global_transform_data_.rotation_rad = ground.ConvertRotationGroundToGlobalWithGroundPosition(ground_transform_data_.position, ground_transform_data_.rotation_rad);
				rotation_last_update = CoordinateSystem::None; //ここで更新したら再び回転度を更新する必要はないのでNoneに設定しておく。
			}
			position_last_update = CoordinateSystem::None;
		}
		if (rotation_last_update == CoordinateSystem::Ground) {
			global_transform_data_.rotation_rad = ground.ConvertRotationGroundToGlobalWithGroundPosition(ground_transform_data_.position, ground_transform_data_.rotation_rad);
			rotation_last_update = CoordinateSystem::None;
		}
	}

	void RootTransform2DCore::UpdateTransformDataGroundByGlobal() {
		auto& ground = this->ground();
		if (position_last_update == CoordinateSystem::Global) {
			ground_transform_data_.position = ground.ConvertPositionGlobalToGround(global_transform_data_.position);
			position_last_update = CoordinateSystem::None;
		}
		if (rotation_last_update == CoordinateSystem::Global) {
			ground_transform_data_.rotation_rad = ground.ConvertRotationGlobalToGroundWithGroundPosition(ground_transform_data_.position, global_transform_data_.rotation_rad);
			rotation_last_update = CoordinateSystem::None;
		}
	}


	void RootTransform2DCore::UpdatePhysicalDataGlobal() {
		auto& ground = this->ground();
		if (velocity_last_update == CoordinateSystem::Ground) {
			global_velocity_ = ground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), ground_velocity_);
			velocity_last_update = CoordinateSystem::None;
		}
	}


	void RootTransform2DCore::UpdatePhysicalDataGround() {
		auto& ground = this->ground();
		if (velocity_last_update == CoordinateSystem::Global) {
			ground_velocity_ = ground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), global_velocity_);
			velocity_last_update = CoordinateSystem::None;
		}
	}

	const Vector2Dd& RootTransform2DCore::position() const {
		const_cast<RootTransform2DCore*>(this)->UpdateTransformDataGroundByGlobal();
		return ground_transform_data_.position;
	}

	void RootTransform2DCore::position(const Vector2Dd& pos) {
		ground_transform_data_.position = pos;
		position_last_update = CoordinateSystem::Ground;
	}

	const Vector2Dd& RootTransform2DCore::scale() const {
		return ground_transform_data_.scale;
	}

	void RootTransform2DCore::scale(const Vector2Dd& s) {
		ground_transform_data_.scale = s;
	}

	const double RootTransform2DCore::rotation_rad() const {
		const_cast<RootTransform2DCore*>(this)->UpdateTransformDataGroundByGlobal();
		return ground_transform_data_.rotation_rad;
	}

	void RootTransform2DCore::rotation_rad(double rota_rad) {
		ground_transform_data_.rotation_rad = rota_rad;
		rotation_last_update = CoordinateSystem::Ground;
	}

	const Vector2Dd& RootTransform2DCore::global_position() const {
		const_cast<RootTransform2DCore*>(this)->UpdateTransformDataGlobalByGround();
		return global_transform_data_.position;
	}

	void RootTransform2DCore::global_position(const Vector2Dd& pos) {
		global_transform_data_.position = pos;
		position_last_update = CoordinateSystem::Global;
	}

	const double RootTransform2DCore::global_rotation_rad() const {
		const_cast<RootTransform2DCore*>(this)->UpdateTransformDataGlobalByGround();
		return global_transform_data_.rotation_rad;
	}

	void RootTransform2DCore::global_rotation_rad(double rota_rad) {
		global_transform_data_.rotation_rad = rota_rad;
		rotation_last_update = CoordinateSystem::Global;
	}

	const Vector2Dd& RootTransform2DCore::velocity() const {
		const_cast<RootTransform2DCore*>(this)->UpdatePhysicalDataGround();
		return ground_velocity_;
	}

	void RootTransform2DCore::velocity(const Vector2Dd& vel) {
		ground_velocity_ = vel;
		velocity_last_update = CoordinateSystem::Ground;
	}

	const double RootTransform2DCore::rotation_velocity_rad() const {
		return rotation_velocity_rad_;
	}

	void RootTransform2DCore::rotation_velocity_rad(double rota_vel_rad) {
		rotation_velocity_rad_ = rota_vel_rad;
	}

	const Vector2Dd& RootTransform2DCore::global_velocity()const {
		const_cast<RootTransform2DCore*>(this)->UpdatePhysicalDataGlobal();
		return global_velocity_;
	}

	void RootTransform2DCore::global_velocity(const Vector2Dd& vel) {
		global_velocity_ = vel;
		velocity_last_update = CoordinateSystem::Global;
	}

	CGround2D& RootTransform2DCore::ground() {
		return *ground_;
	}

	utility::WeakPointer<CGround2D> RootTransform2DCore::GetGround() const {
		return ground_;
	}

	void RootTransform2DCore::SetGround(const utility::WeakPointer<CGround2D>& g, bool keep_global_position) {
		if (keep_global_position) {
			UpdateTransformDataGlobalByGround();
			UpdatePhysicalDataGlobal();
		} else {
			UpdateTransformDataGroundByGlobal();
			UpdatePhysicalDataGround();
		}

		ground_ = g;

		if (keep_global_position) {
			position_last_update = CoordinateSystem::Global;
			rotation_last_update = CoordinateSystem::Global;
			velocity_last_update = CoordinateSystem::Global;
			UpdateTransformDataGroundByGlobal();
			UpdatePhysicalDataGround();
		} else {
			position_last_update = CoordinateSystem::Ground;
			rotation_last_update = CoordinateSystem::Ground;
			velocity_last_update = CoordinateSystem::Ground;
			UpdateTransformDataGlobalByGround();
			UpdatePhysicalDataGlobal();
		}
	}

	void RootTransform2DCore::SetParent(Transform2DCore* parent) {
		debug::SystemLog::instance().Log(debug::LogLevel::Fatal, __FUNCTION__, "ルートトランスフォームに親は設定できません。");
	}
}