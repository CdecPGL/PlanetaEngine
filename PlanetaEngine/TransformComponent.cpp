#include "TransformComponent.h"
#include "GroundComponent.h"
#include "DumyGroundComponent.h"
#include <cassert>

namespace planeta_engine {
	namespace components {

		TransformComponent::TransformComponent():ground_(GetDumyGroundComponent_())
		{
			
		}


		void TransformComponent::Move(const Vector2D<double>& mov_pos) {
			Offset(position(), mov_pos);
		}


		void TransformComponent::Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset) {
			position(base_pos + ground().NormalizeGroundVectorWithGroundPosition(base_pos, offset));
		}


		void TransformComponent::Accelerate(const Vector2D<double>& acceleration) {
			velocity(velocity() + acceleration);
		}


		void TransformComponent::GlobalAccelerate(const Vector2D<double>& global_acceleration) {
			global_velocity(global_velocity() + global_acceleration);
		}

		void TransformComponent::ApplyVelocity()
		{
			Move(velocity());
			rotation_rad(rotation_rad() + rotation_velocity_rad());
		}

		void TransformComponent::UpdateTransformDataGlobalByGround()
		{
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

		void TransformComponent::UpdateTransformDataGroundByGlobal()
		{
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


		void TransformComponent::UpdatePhysicalDataGlobal() {
			auto& ground = this->ground();
			if (velocity_last_update == CoordinateSystem::Ground) {
				global_velocity_ = ground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), ground_velocity_);
				velocity_last_update = CoordinateSystem::None;
			}
		}


		void TransformComponent::UpdatePhysicalDataGround() {
			auto& ground = this->ground();
			if (velocity_last_update == CoordinateSystem::Global) {
				ground_velocity_ = ground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), global_velocity_);
				velocity_last_update = CoordinateSystem::None;
			}
		}

		const Vector2D<double>& TransformComponent::position() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGroundByGlobal();
			return ground_transform_data_.position;
		}

		void TransformComponent::position(const Vector2D<double>& pos) {
			ground_transform_data_.position = pos;
			position_last_update = CoordinateSystem::Ground;
		}

		const Vector2D<double>& TransformComponent::scale() const {
			return ground_transform_data_.scale;
		}

		void TransformComponent::scale(const Vector2D<double>& s) {
			ground_transform_data_.scale = s;
		}

		const double TransformComponent::rotation_rad() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGroundByGlobal();
			return ground_transform_data_.rotation_rad;
		}

		void TransformComponent::rotation_rad(double rota_rad) {
			ground_transform_data_.rotation_rad = rota_rad;
			rotation_last_update = CoordinateSystem::Ground;
		}

		const Vector2D<double>& TransformComponent::global_position() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGlobalByGround();
			return global_transform_data_.position;
		}

		void TransformComponent::global_position(const Vector2D<double>& pos) {
			global_transform_data_.position = pos;
			position_last_update = CoordinateSystem::Global;
		}

		const double TransformComponent::global_rotation_rad() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGlobalByGround();
			return global_transform_data_.rotation_rad;
		}

		void TransformComponent::global_rotation_rad(double rota_rad) {
			global_transform_data_.rotation_rad = rota_rad;
			rotation_last_update = CoordinateSystem::Global;
		}

		const Vector2D<double>& TransformComponent::velocity() const {
			const_cast<TransformComponent*>(this)->UpdatePhysicalDataGround();
			return ground_velocity_;
		}

		void TransformComponent::velocity(const Vector2D<double>& vel) {
			ground_velocity_ = vel;
			velocity_last_update = CoordinateSystem::Ground;
		}

		const double TransformComponent::rotation_velocity_rad() const {
			return rotation_velocity_rad_;
		}

		void TransformComponent::rotation_velocity_rad(double rota_vel_rad) {
			rotation_velocity_rad_ = rota_vel_rad;
		}

		const Vector2D<double>& TransformComponent::global_velocity()const {
			const_cast<TransformComponent*>(this)->UpdatePhysicalDataGlobal();
			return global_velocity_;
		}

		void TransformComponent::global_velocity(const Vector2D<double>& vel) {
			global_velocity_ = vel;
			velocity_last_update = CoordinateSystem::Global;
		}

		const GroundComponent& TransformComponent::ground() const {
			return *ground_;
		}

		GroundComponent& TransformComponent::ground() {
			return *ground_;
		}

		utility::WeakPointer<GroundComponent> TransformComponent::GetGround() const {
			return ground_;
		}

		void TransformComponent::SetGround(const utility::WeakPointer<GroundComponent>& g, bool keep_global_position) {
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

		std::shared_ptr<components::GroundComponent> TransformComponent::GetDumyGroundComponent_() {
			static std::shared_ptr<components::DumyGroundComponent> dgc = std::make_shared<components::DumyGroundComponent>();
			return dgc;
		}

	}
}