#pragma once

#include "planeta/planeta_engine.hpp"

class t_camera_follower final : public plnt::task {
public:
	using super = task;
	t_camera_follower() noexcept;

	enum class mode { ground_base, target_base };

	bool change_mode(mode mode, int interpolate_frame = 0);
	bool set_camera(const plnt::weak_pointer<plnt::i_game_object> &game_object);
	bool set_target(const plnt::weak_pointer<plnt::i_game_object> &game_object, int interpolate_frame = 0);
	void update() override;

private:
	plnt::weak_pointer<plnt::c_camera_2d> camera_;
	plnt::weak_pointer<plnt::c_transform_2d> target_;

	mode mode_;

	bool interpolate_trigger_ = false; //補間トリガー
	int interpolate_frame_counter_ = 0;
	int interpolate_frame_ = 1;
	double calc_interpolate_ratio() const; //現在の補間割合を計算

	struct camera_view_point_info {
		plnt::vector_2dd pos;
		double rotation_rad = 0.0;
		double scale = 0.0;
		camera_view_point_info operator +(const camera_view_point_info &) const;
		camera_view_point_info operator -(const camera_view_point_info &) const;
		camera_view_point_info operator *(double) const;
	};

	camera_view_point_info objective_view_point_; //目標視点
	camera_view_point_info current_view_point_; //現在の視点
	camera_view_point_info view_point_interpolate_; //視点補間値

	std::function<double(double)> interpolation_func_;
};

PE_TASK_CLASS(t_camera_follower);
