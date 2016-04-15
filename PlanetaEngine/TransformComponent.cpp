#include "TransformComponent.h"
#include "TransformCore.h"
#include <cassert>

#include "RootTransformCore.h"

namespace planeta_engine {
	namespace components {
		TransformComponent::TransformComponent():core_(std::make_unique<RootTransformCore>()) {};

		TransformComponent::~TransformComponent() = default;

		void TransformComponent::Move(const Vector2Dd& mov_pos) {
			Offset(position(), mov_pos);
		}

		void TransformComponent::Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) {
			core_->Offset(base_pos, offset);
		}

		void TransformComponent::Accelerate(const Vector2Dd& acceleration) {
			velocity(velocity() + acceleration);
		}

		void TransformComponent::GlobalAccelerate(const Vector2Dd& global_acceleration) {
			global_velocity(global_velocity() + global_acceleration);
		}

		void TransformComponent::ApplyVelocity()
		{
			Move(velocity());
			rotation_rad(rotation_rad() + rotation_velocity_rad());
		}

		const Vector2Dd& TransformComponent::position() const {
			return core_->position();
		}

		TransformComponent&  TransformComponent::position(const Vector2Dd& pos) {
			core_->position(pos);
			return *this;
		}

		const Vector2Dd& TransformComponent::scale() const {
			return core_->scale();
		}

		TransformComponent&  TransformComponent::scale(const Vector2Dd& s) {
			core_->scale(s);
			return *this;
		}

		const double TransformComponent::rotation_rad() const {
			return core_->rotation_rad();
		}

		TransformComponent&  TransformComponent::rotation_rad(double rota_rad) {
			core_->rotation_rad(rota_rad);
			return *this;
		}

		const Vector2Dd& TransformComponent::global_position() const {
			return core_->global_position();
		}

		TransformComponent&  TransformComponent::global_position(const Vector2Dd& pos) {
			core_->global_position(pos);
			return *this;
		}

		const double TransformComponent::global_rotation_rad() const {
			return core_->global_rotation_rad();
		}

		TransformComponent&  TransformComponent::global_rotation_rad(double rota_rad) {
			core_->global_rotation_rad(rota_rad);
			return *this;
		}

		const Vector2Dd& TransformComponent::velocity() const {
			return core_->velocity();
		}

		TransformComponent&  TransformComponent::velocity(const Vector2Dd& vel) {
			core_->velocity(vel);
			return *this;
		}

		const double TransformComponent::rotation_velocity_rad() const {
			return core_->rotation_velocity_rad();
		}

		TransformComponent&  TransformComponent::rotation_velocity_rad(double rota_vel_rad) {
			core_->rotation_velocity_rad(rota_vel_rad);
			return *this;
		}

		const Vector2Dd& TransformComponent::global_velocity()const {
			return core_->global_velocity();
		}

		TransformComponent&  TransformComponent::global_velocity(const Vector2Dd& vel) {
			core_->global_velocity(vel);
			return *this;
		}

		const GroundComponent& TransformComponent::ground() const {
			return const_cast<TransformComponent*>(this)->ground();
		}

		GroundComponent& TransformComponent::ground() {
			return core_->ground();
		}

		utility::WeakPointer<GroundComponent> TransformComponent::GetGround() const {
			return core_->GetGround();
		}

		void TransformComponent::SetGround(const utility::WeakPointer<GroundComponent>& g, bool keep_global_position) {
			core_->SetGround(g, keep_global_position);
		}
	}
}