#include <cassert>

#include "RootTransformCore.h"
#include "GroundComponent.h"
#include "DumyGroundComponent.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace {
		utility::WeakPointer<components::DumyGroundComponent> GetDumyGround() {
			static std::shared_ptr<components::DumyGroundComponent> com = std::make_shared<components::DumyGroundComponent>();
			return com;
		}
	}

	RootTransformCore::RootTransformCore() :ground_(GetDumyGround()) {}

	void RootTransformCore::Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) {
		position(base_pos + ground().NormalizeGroundVectorWithGroundPosition(base_pos, offset));
	}

	void RootTransformCore::UpdateTransformDataGlobalByGround() {
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

	void RootTransformCore::UpdateTransformDataGroundByGlobal() {
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


	void RootTransformCore::UpdatePhysicalDataGlobal() {
		auto& ground = this->ground();
		if (velocity_last_update == CoordinateSystem::Ground) {
			global_velocity_ = ground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), ground_velocity_);
			velocity_last_update = CoordinateSystem::None;
		}
	}


	void RootTransformCore::UpdatePhysicalDataGround() {
		auto& ground = this->ground();
		if (velocity_last_update == CoordinateSystem::Global) {
			ground_velocity_ = ground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), global_velocity_);
			velocity_last_update = CoordinateSystem::None;
		}
	}

	const Vector2Dd& RootTransformCore::position() const {
		const_cast<RootTransformCore*>(this)->UpdateTransformDataGroundByGlobal();
		return ground_transform_data_.position;
	}

	void RootTransformCore::position(const Vector2Dd& pos) {
		ground_transform_data_.position = pos;
		position_last_update = CoordinateSystem::Ground;
	}

	const Vector2Dd& RootTransformCore::scale() const {
		return ground_transform_data_.scale;
	}

	void RootTransformCore::scale(const Vector2Dd& s) {
		ground_transform_data_.scale = s;
	}

	const double RootTransformCore::rotation_rad() const {
		const_cast<RootTransformCore*>(this)->UpdateTransformDataGroundByGlobal();
		return ground_transform_data_.rotation_rad;
	}

	void RootTransformCore::rotation_rad(double rota_rad) {
		ground_transform_data_.rotation_rad = rota_rad;
		rotation_last_update = CoordinateSystem::Ground;
	}

	const Vector2Dd& RootTransformCore::global_position() const {
		const_cast<RootTransformCore*>(this)->UpdateTransformDataGlobalByGround();
		return global_transform_data_.position;
	}

	void RootTransformCore::global_position(const Vector2Dd& pos) {
		global_transform_data_.position = pos;
		position_last_update = CoordinateSystem::Global;
	}

	const double RootTransformCore::global_rotation_rad() const {
		const_cast<RootTransformCore*>(this)->UpdateTransformDataGlobalByGround();
		return global_transform_data_.rotation_rad;
	}

	void RootTransformCore::global_rotation_rad(double rota_rad) {
		global_transform_data_.rotation_rad = rota_rad;
		rotation_last_update = CoordinateSystem::Global;
	}

	const Vector2Dd& RootTransformCore::velocity() const {
		const_cast<RootTransformCore*>(this)->UpdatePhysicalDataGround();
		return ground_velocity_;
	}

	void RootTransformCore::velocity(const Vector2Dd& vel) {
		ground_velocity_ = vel;
		velocity_last_update = CoordinateSystem::Ground;
	}

	const double RootTransformCore::rotation_velocity_rad() const {
		return rotation_velocity_rad_;
	}

	void RootTransformCore::rotation_velocity_rad(double rota_vel_rad) {
		rotation_velocity_rad_ = rota_vel_rad;
	}

	const Vector2Dd& RootTransformCore::global_velocity()const {
		const_cast<RootTransformCore*>(this)->UpdatePhysicalDataGlobal();
		return global_velocity_;
	}

	void RootTransformCore::global_velocity(const Vector2Dd& vel) {
		global_velocity_ = vel;
		velocity_last_update = CoordinateSystem::Global;
	}

	components::GroundComponent& RootTransformCore::ground() {
		return *ground_;
	}

	utility::WeakPointer<components::GroundComponent> RootTransformCore::GetGround() const {
		return ground_;
	}

	void RootTransformCore::SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position) {
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

	void RootTransformCore::SetParent(TransformCore* parent) {
		debug::SystemLog::instance().Log(debug::LogLevel::Fatal, __FUNCTION__, "ルートトランスフォームに親は設定できません。");
	}
}