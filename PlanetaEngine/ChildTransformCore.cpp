#include "ChildTransformCore.h"
#include "SystemLog.h"

namespace planeta_engine {

	const Vector2D<double>& ChildTransformCore::position() const {
		return local_transform_data_.position;
	}

	void ChildTransformCore::position(const Vector2D<double>& pos) {
		
	}

	const Vector2D<double>& ChildTransformCore::scale() const {
		return local_transform_data_.scale;
	}

	void ChildTransformCore::scale(const Vector2D<double>& s) {

	}

	const double ChildTransformCore::rotation_rad() const {
		return local_transform_data_.rotation_rad;
	}

	void ChildTransformCore::rotation_rad(double rota_rad) {

	}

	const Vector2D<double>& ChildTransformCore::global_position() const {
		return global_transform_data_.position;
	}

	void ChildTransformCore::global_position(const Vector2D<double>& pos) {

	}

	const double ChildTransformCore::global_rotation_rad() const {
		return global_transform_data_.rotation_rad;
	}

	void ChildTransformCore::global_rotation_rad(double rota_rad) {

	}

	const Vector2D<double>& ChildTransformCore::velocity() const {
		return Vector2D<double>();
	}

	void ChildTransformCore::velocity(const Vector2D<double>& vel) {

	}

	const double ChildTransformCore::rotation_velocity_rad() const {
		return 0;
	}

	void ChildTransformCore::rotation_velocity_rad(double rota_vel_rad) {

	}

	const Vector2D<double>& ChildTransformCore::global_velocity() const {
		return Vector2D<double>();
	}

	void ChildTransformCore::global_velocity(const Vector2D<double>& vel) {

	}

	planeta_engine::components::GroundComponent& ChildTransformCore::ground() {
		return *GetGround();
	}

	planeta_engine::utility::WeakPointer<components::GroundComponent> ChildTransformCore::GetGround() const {
		return nullptr;
	}

	void ChildTransformCore::SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position) {
		debug::SystemLog::instance().Log(debug::LogLevel::Fatal, __FUNCTION__, "ChildTransform‚Å‚Í’nŒ`‚ðÝ’è‚Å‚«‚Ü‚¹‚ñB");
	}

	void ChildTransformCore::SetParent(TransformCore* parent) {
		parent_ = parent;
	}

	void ChildTransformCore::UpdateGlobalFromLocal() {
		local_transform_data_.position;

		global_transform_data_.position = parent_->global_position();
	}

	void ChildTransformCore::UpdateLocalFromGlobal() {

	}

}