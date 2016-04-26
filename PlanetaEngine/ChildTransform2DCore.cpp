#include "ChildTransform2DCore.h"
#include "SystemLog.h"

namespace planeta_engine {

	const Vector2Dd& ChildTransform2DCore::position() const {
		return local_transform_data_.position;
	}

	void ChildTransform2DCore::position(const Vector2Dd& pos) {
		
	}

	const Vector2Dd& ChildTransform2DCore::scale() const {
		return local_transform_data_.scale;
	}

	void ChildTransform2DCore::scale(const Vector2Dd& s) {

	}

	const double ChildTransform2DCore::rotation_rad() const {
		return local_transform_data_.rotation_rad;
	}

	void ChildTransform2DCore::rotation_rad(double rota_rad) {

	}

	const Vector2Dd& ChildTransform2DCore::global_position() const {
		return global_transform_data_.position;
	}

	void ChildTransform2DCore::global_position(const Vector2Dd& pos) {

	}

	const double ChildTransform2DCore::global_rotation_rad() const {
		return global_transform_data_.rotation_rad;
	}

	void ChildTransform2DCore::global_rotation_rad(double rota_rad) {

	}

	const Vector2Dd& ChildTransform2DCore::velocity() const {
		return Vector2Dd();
	}

	void ChildTransform2DCore::velocity(const Vector2Dd& vel) {

	}

	const double ChildTransform2DCore::rotation_velocity_rad() const {
		return 0;
	}

	void ChildTransform2DCore::rotation_velocity_rad(double rota_vel_rad) {

	}

	const Vector2Dd& ChildTransform2DCore::global_velocity() const {
		return Vector2Dd();
	}

	void ChildTransform2DCore::global_velocity(const Vector2Dd& vel) {

	}

	planeta_engine::components::CGround2D& ChildTransform2DCore::ground() {
		return *GetGround();
	}

	planeta_engine::utility::WeakPointer<components::CGround2D> ChildTransform2DCore::GetGround() const {
		return nullptr;
	}

	void ChildTransform2DCore::SetGround(const utility::WeakPointer<components::CGround2D>& g, bool keep_global_position) {
		debug::SystemLog::instance().Log(debug::LogLevel::Fatal, __FUNCTION__, "ChildTransform‚Å‚Í’nŒ`‚ðÝ’è‚Å‚«‚Ü‚¹‚ñB");
	}

	void ChildTransform2DCore::SetParent(Transform2DCore* parent) {
		parent_ = parent;
	}

	void ChildTransform2DCore::UpdateGlobalFromLocal() {
		local_transform_data_.position;

		global_transform_data_.position = parent_->global_position();
	}

	void ChildTransform2DCore::UpdateLocalFromGlobal() {

	}

}