#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>

#include "GameObjectStandardComponent.h"
#include "WeakPointer.h"
#include "RectAngle.h"

namespace planeta {
	class CDrawGraph2D;
	class CGraph2DAnimator : public GameObjectStandardComponent {
	public:
		/*�t���[���f�[�^�̃^�C�v<�\���t���[����,�\���͈�,�g��x,��]�x(���W�A��)>*/
		using FrameDataType = std::tuple<unsigned int, util::RectAngle<int>, Vector2Dd, double>;
		/*�P�����t���[���f�[�^�̃^�C�v<�\���t���[����,�\���͈�>*/
		using SimpleFrameDataType = std::pair<unsigned int, util::RectAngle<int>>;
		/*�A�j���[�V�����Z�b�g(�t��)*/
		void SetAnimation(const std::string& anim_name, const std::vector<FrameDataType>& frames);
		/*�A�j���[�V�����Z�b�g(�V���v��)*/
		void SetAnimation(const std::string& anim_name, const std::vector<SimpleFrameDataType>& frames);
		/*�w�薼�̃A�j���[�V�����̍Đ����J�n����*/
		bool StartAnimation(const std::string& anim_name, bool roop_flag);
		/*�A�j���[�V�������Đ����Ȃ��~����*/
		void StopAnimation();
		/*�Đ����̃A�j���[�V���������擾����*/
		const std::string GetPlayingAnimationName()const { return is_playing_ ? current_animation_name_ : ""; }
	private:
		util::NonOwingPointer<CDrawGraph2D> draw_graph_component_;
		unsigned int frame_counter_ = 0;
		bool is_playing_ = false;
		bool roop_flag_ = false;
		unsigned int current_frame_ = 0;
		std::unordered_map<std::string, std::shared_ptr<std::vector<FrameDataType>>> animations_;
		std::shared_ptr<std::vector<FrameDataType>> current_animation_;
		std::string current_animation_name_;
		void SetCurrentFrameDrawAreaToDrawGraphComponent_();
		bool OnInitialized() override;
		void UpdateAnimation();
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CGraph2DAnimator);
}