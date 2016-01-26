#include "TransformComponent.h"
#include "GroundComponent.h"
#include <cassert>

namespace planeta_engine {
	namespace components {

		TransformComponent::TransformComponent()
		{
			
		}


		void TransformComponent::Move(const Vector2D<double>& mov_pos) {
			Offset(position(), mov_pos);
		}


		void TransformComponent::Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset) {
			position(base_pos + belonging_ground_->NormalizeGroundVectorWithGroundPosition(base_pos, offset));
		}

		void TransformComponent::ApplyVelocity()
		{
			Move(velocity());
		}

		void TransformComponent::UpdateTransformDataGlobalByGround()
		{
			auto& ground = *belonging_ground_;
			if (ground_transform_data_.position_updated) {
				ground_transform_data_.position = ground.ConvertPositionGlobalToGround(global_transform_data_.position);
			}
			if (ground_transform_data_.rotation_updated || ground_transform_data_.position_updated) {
				ground_transform_data_.rotation_rad = ground.ConvertRotationGlobalToGroundWithGroundPosition(ground_transform_data_.position, global_transform_data_.rotation_rad);
			}
			global_transform_data_.position_updated = false;
			global_transform_data_.rotation_updated = false;
			ground_transform_data_.position_updated = false;
			ground_transform_data_.rotation_updated = false;
		}

		void TransformComponent::UpdateTransformDataGroundByGlobal()
		{
			auto& ground = *belonging_ground_;
			if (global_transform_data_.position_updated) {
				global_transform_data_.position = ground.ConvertPositionGroundToGlobal(ground_transform_data_.position);
			}
			if (global_transform_data_.rotation_updated || global_transform_data_.position_updated) {
				global_transform_data_.rotation_rad = ground.ConvertRotationGroundToGlobalWithGroundPosition(ground_transform_data_.position, ground_transform_data_.rotation_rad);
			}
			global_transform_data_.position_updated = false;
			global_transform_data_.rotation_updated = false;
			ground_transform_data_.position_updated = false;
			ground_transform_data_.rotation_updated = false;
		}


		void TransformComponent::UpdatePhysicalDataGlobal() {
			auto& ground = *belonging_ground_;
			if (ground_velocity_updated_) {
				//地形座標の正規化を行うためにここでは地形速度にプロパティを通してアクセスする。
				global_velocity_ = ground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), velocity());
			}
			ground_velocity_updated_ = false;
			global_velocity_updated_ = false;
		}


		void TransformComponent::UpdatePhysicalDataGround() {
			auto& ground = *belonging_ground_;
			if (global_velocity_updated_) {
				ground_velocity_ = ground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), global_velocity_);
			} else if (ground_velocity_updated_) {
				ground_velocity_ = ground.NormalizeGroundVectorWithGroundPosition(position(), global_velocity_);
			}
			ground_velocity_updated_ = false;
			global_velocity_updated_ = false;
		}

		const Vector2D<double>& TransformComponent::position() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGroundByGlobal();
			return ground_transform_data_.position;
		}

		void TransformComponent::position(const Vector2D<double>& pos) {
			ground_transform_data_.position = pos;
			ground_transform_data_.position_updated = true;
			global_transform_data_.position_updated = false;
		}

		const Vector2D<double>& TransformComponent::scale() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGroundByGlobal();
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
			ground_transform_data_.rotation_updated = true;
			global_transform_data_.rotation_updated = false;
		}

		const Vector2D<double>& TransformComponent::global_position() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGlobalByGround();
			return global_transform_data_.position;
		}

		void TransformComponent::global_position(const Vector2D<double>& pos) {
			global_transform_data_.position = pos;
			global_transform_data_.position_updated = true;
			ground_transform_data_.position_updated = false;
		}

		const double TransformComponent::global_rotation_rad() const {
			const_cast<TransformComponent*>(this)->UpdateTransformDataGlobalByGround();
			return global_transform_data_.rotation_rad;
		}

		void TransformComponent::global_rotation_rad(double rota_rad) {
			global_transform_data_.rotation_rad = rota_rad;
			global_transform_data_.rotation_updated = true;
			ground_transform_data_.rotation_updated = false;
		}

		const Vector2D<double>& TransformComponent::velocity() const {
			const_cast<TransformComponent*>(this)->UpdatePhysicalDataGround();
			return ground_velocity_;
		}

		void TransformComponent::velocity(const Vector2D<double>& vel) {
			ground_velocity_ = vel;
			ground_velocity_updated_ = true;
			global_velocity_updated_ = false;
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
			global_velocity_updated_ = true;
			ground_velocity_updated_ = false;
		}
	}
}