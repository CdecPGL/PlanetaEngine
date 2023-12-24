#pragma once

#include "planeta/PlanetaEngine.hpp"

class TCameraFollower : public plnt::Task {
public:
	using Super = plnt::Task;
	TCameraFollower()noexcept;
	enum class Mode { GroundBase, TargetBase };
	bool ChangeMode(Mode mode, int interp_frame = 0);
	bool SetCamera(plnt::WeakPointer<plnt::IGameObject> game_object);
	bool SetTarget(plnt::WeakPointer<plnt::IGameObject> game_object, int interp_frame = 0);
	virtual void Update() override;
private:
	plnt::WeakPointer<plnt::CCamera2D> camera_;
	plnt::WeakPointer<plnt::CTransform2D> target_;

	Mode _mode;

	bool interp_trigger_ = false; //補間トリガー
	int interp_frame_cntr_ = 0;
	int interp_frame_ = 1;
	double calc_interp_ratio_(); //現在の補間割合を計算

	struct CameraViewPointInfo {
		plnt::Vector2Dd pos;
		double rotation_rad = 0.0;
		double scale = 0.0;
		CameraViewPointInfo operator +(const CameraViewPointInfo&)const;
		CameraViewPointInfo operator -(const CameraViewPointInfo&)const;
		CameraViewPointInfo operator *(double)const;
	};

	CameraViewPointInfo objective_view_point_; //目標視点
	CameraViewPointInfo current_view_point_; //現在の視点
	CameraViewPointInfo view_point_interp_; //視点補間値

	std::function<double(double)> interpolation_func_;
};
PE_TASK_CLASS(TCameraFollower);