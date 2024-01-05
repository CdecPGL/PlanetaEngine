#include "planeta/math.hpp"

#include "t_camera_follower.hpp"

using namespace plnt;

t_camera_follower::t_camera_follower() noexcept : mode_(),
                                              interpolation_func_([](const double x) {
	                                              return pow((1 - sin(x * math::pi / 2)), 2);
                                              }) {}

bool t_camera_follower::set_camera(const weak_pointer<i_game_object> &game_object) {
	if (const auto gi_pc = game_object->get_component<c_camera_2d>()) {
		camera_ = gi_pc;
		return true;
	}
	PE_LOG_ERROR("CCamera2Dを持たないゲームオブジェクトが指定されました。");
	return false;
}

bool t_camera_follower::set_target(const weak_pointer<i_game_object> &game_object,
                                 [[maybe_unused]] int interpolate_frame) {
	if (const auto gi_pc = game_object->get_component<c_transform_2d>()) {
		target_ = gi_pc;
		return true;
	}
	PE_LOG_ERROR("CTransform2Dを持たないゲームオブジェクトが指定されました。");
	return false;
}

void t_camera_follower::update() {
	//ターゲットの位置を目標視点とする
	if (target_) {
		objective_view_point_.scale = 1.0;
		objective_view_point_.pos = target_->position();
		objective_view_point_.rotation_rad = target_->rotation_rad();
	}
	//現在の視点を求める
	current_view_point_ = objective_view_point_;
	//カメラに現在の視点を反映
	if (camera_) {
		camera_->expansion(current_view_point_.scale);
		camera_->position(current_view_point_.pos);
		camera_->rotation_rad(current_view_point_.rotation_rad);
	}
}

double t_camera_follower::calc_interpolate_ratio() const {
	return interpolation_func_(
		static_cast<double>(interpolate_frame_counter_) / static_cast<double>(interpolate_frame_));
}

t_camera_follower::camera_view_point_info t_camera_follower::camera_view_point_info::operator +(
	const camera_view_point_info &in) const {
	camera_view_point_info out;
	out.pos = pos + in.pos;
	out.rotation_rad = rotation_rad + in.rotation_rad;
	out.scale = scale + in.scale;
	return out;
}

t_camera_follower::camera_view_point_info t_camera_follower::camera_view_point_info::operator -(
	const camera_view_point_info &in) const {
	camera_view_point_info out;
	out.pos = pos - in.pos;
	out.rotation_rad = rotation_rad - in.rotation_rad;
	out.scale = scale - in.scale;
	return out;
}

t_camera_follower::camera_view_point_info t_camera_follower::camera_view_point_info::operator *(const double in) const {
	camera_view_point_info out;
	out.pos = pos * in;
	out.rotation_rad = rotation_rad * in;
	out.scale = scale * in;
	return out;
}
