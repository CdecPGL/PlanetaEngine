#include "planeta/Math.hpp"

#include "TCameraFollower.hpp"

using namespace plnt;

TCameraFollower::TCameraFollower() noexcept :interpolation_func_([](double x) {return pow((1 - sin(x*math::PI / 2)), 2); }){}

bool TCameraFollower::SetCamera(plnt::WeakPointer<plnt::IGameObject> game_object) {
	auto gi_pc = game_object->GetComponent<CCamera2D>();
	if (gi_pc) {
		camera_ = gi_pc;
		return true;
	} else {
		PE_LOG_ERROR("CCamera2D�������Ȃ��Q�[���I�u�W�F�N�g���w�肳��܂����B");
		return false;
	}
}

bool TCameraFollower::SetTarget(plnt::WeakPointer<plnt::IGameObject> game_object, [[maybe_unused]] int interp_frame) {
	auto gi_pc = game_object->GetComponent<CTransform2D>();
	if (gi_pc) {
		target_ = gi_pc;
		return true;
	} else {
		PE_LOG_ERROR("CTransform2D�������Ȃ��Q�[���I�u�W�F�N�g���w�肳��܂����B");
		return false;
	}
}

void TCameraFollower::Update() {
	//�^�[�Q�b�g�̈ʒu��ڕW���_�Ƃ���
	if (target_) {
		objective_view_point_.scale = 1.0;
		objective_view_point_.pos = target_->position();
		objective_view_point_.rotation_rad = target_->rotation_rad();
	}
	//���݂̎��_�����߂�
	current_view_point_ = objective_view_point_;
	//�J�����Ɍ��݂̎��_�𔽉f
	if (camera_) {
		camera_->expansion(current_view_point_.scale);
		camera_->position(current_view_point_.pos);
		camera_->rotation_rad(current_view_point_.rotation_rad);
	}
}

double TCameraFollower::calc_interp_ratio_() {
	return interpolation_func_((double)interp_frame_cntr_ / (double)interp_frame_);
}

TCameraFollower::CameraViewPointInfo TCameraFollower::CameraViewPointInfo::operator + (const CameraViewPointInfo& in)const {
	CameraViewPointInfo out;
	out.pos = pos + in.pos;
	out.rotation_rad = rotation_rad + in.rotation_rad;
	out.scale = scale + in.scale;
	return out;
}
TCameraFollower::CameraViewPointInfo TCameraFollower::CameraViewPointInfo::operator - (const CameraViewPointInfo& in)const {
	CameraViewPointInfo out;
	out.pos = pos - in.pos;
	out.rotation_rad = rotation_rad - in.rotation_rad;
	out.scale = scale - in.scale;
	return out;
}
TCameraFollower::CameraViewPointInfo TCameraFollower::CameraViewPointInfo::operator * (double in)const {
	CameraViewPointInfo out;
	out.pos = pos * in;
	out.rotation_rad = rotation_rad * in;
	out.scale = scale * in;
	return out;
}
