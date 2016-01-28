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
			rotation_rad(rotation_rad() + rotation_velocity_rad_);
		}

		void TransformComponent::UpdateTransformDataGlobalByGround()
		{
			auto& ground = this->ground();
			if (ground_transform_data_.position_updated) {
				global_transform_data_.position = ground.ConvertPositionGroundToGlobal(ground_transform_data_.position);
			}
			if (ground_transform_data_.rotation_updated || ground_transform_data_.position_updated) {
				global_transform_data_.rotation_rad = ground.ConvertRotationGroundToGlobalWithGroundPosition(ground_transform_data_.position, ground_transform_data_.rotation_rad);
			}
			global_transform_data_.position_updated = false;
			global_transform_data_.rotation_updated = false;
			ground_transform_data_.position_updated = false;
			ground_transform_data_.rotation_updated = false;
		}

		void TransformComponent::UpdateTransformDataGroundByGlobal()
		{
			auto& ground = this->ground();
			if (global_transform_data_.position_updated) {
				ground_transform_data_.position = ground.ConvertPositionGlobalToGround(global_transform_data_.position);
			}
			if (global_transform_data_.rotation_updated || global_transform_data_.position_updated) {
				ground_transform_data_.rotation_rad = ground.ConvertRotationGlobalToGroundWithGroundPosition(ground_transform_data_.position, global_transform_data_.rotation_rad);
			}
			global_transform_data_.position_updated = false;
			global_transform_data_.rotation_updated = false;
			ground_transform_data_.position_updated = false;
			ground_transform_data_.rotation_updated = false;
		}


		void TransformComponent::UpdatePhysicalDataGlobal() {
			auto& ground = this->ground();
			if (ground_velocity_updated_) {
				global_velocity_ = ground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), ground_velocity_);
			}
			ground_velocity_updated_ = false;
			global_velocity_updated_ = false;
		}


		void TransformComponent::UpdatePhysicalDataGround() {
			auto& ground = this->ground();
			if (global_velocity_updated_) {
				ground_velocity_ = ground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), global_velocity_);
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
				global_transform_data_.position_updated = true;
				global_transform_data_.rotation_updated = true;
				global_velocity_updated_ = true;
				UpdateTransformDataGroundByGlobal();
				UpdatePhysicalDataGround();
			} else {
				ground_transform_data_.position_updated = true;
				ground_transform_data_.rotation_updated = true;
				ground_velocity_updated_ = true;
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