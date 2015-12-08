#include "GraphAnimatorComponent.h"
#include "IGameObjectAccessor.h"
#include "DrawGraphComponent.h"
#include "SystemLog.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace components {
		bool planeta_engine::components::GraphAnimatorComponent::Initialize_()
		{
			draw_graph_component_ = game_object().GetComponent<DrawGraphComponent>();
			if (draw_graph_component_) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("DrawGraphComponent���擾�ł��܂���ł����B", "GraphAnimatorComponent::Initialize_");
				return false;
			}
			
		}

		void planeta_engine::components::GraphAnimatorComponent::Update_()
		{
			if (is_playing_) {
				if (current_animation_ == nullptr) { is_playing_ = false; }
				else {
					//�t���[���J�E���g��0�Ȃ�A�`��R���|�[�l���g���X�V����
					if (frame_counter_ == 0) { SetCurrentFrameDrawAreaToDrawGraphComponent_(); }
					++frame_counter_;
					//�t���[���J�E���g���A���݂̃t���[���̕\���t���[���ȏゾ�����玟��
					if (frame_counter_ >= std::get<0>((*current_animation_)[current_frame_])) {
						++current_frame_;
						frame_counter_ = 0;
						//�V�����t���[�������݂̃A�j���[�V�����̃t���[�����𒴂��Ă�����
						if (current_frame_ >= (*current_animation_).size()) {
							if (roop_flag_) { //���[�v�t���O�������Ă���Ƃ��̓t���[����0�ɖ߂�
								current_frame_ = 0;
							}
							else { //�����Ă��Ȃ��Ƃ��͍Đ����~����
								is_playing_ = false;
							}
						}
						else { //�Ȃɂ��Ȃ��V�����t���[���Ɉڍs����
						}
					}
				}
			}
		}

		void GraphAnimatorComponent::SetAnimation(const std::string& anim_name, const std::vector<FrameDataType>& frames)
		{
			std::shared_ptr<std::vector<FrameDataType>> new_frames = std::make_shared<std::vector<FrameDataType>>();
			*new_frames = frames;
			animations_[anim_name] = new_frames;
		}

		void GraphAnimatorComponent::SetAnimation(const std::string& anim_name, const std::vector<SimpleFrameDataType>& frames)
		{
			std::shared_ptr<std::vector<FrameDataType>> new_frames = std::make_shared<std::vector<FrameDataType>>();
			new_frames->resize(frames.size());
			for (unsigned int i = 0; i < frames.size(); ++i) {
				(*new_frames)[i] = FrameDataType(frames[i].first, frames[i].second, Vector2D<double>(1.0, 1.0), 0.0);
			}
			animations_[anim_name] = new_frames;
		}

		bool GraphAnimatorComponent::StartAnimation(const std::string& anim_name, bool roop_flag)
		{
			auto it = animations_.find(anim_name);
			if (it == animations_.end()) {
				debug::SystemLog::instance().LogError(std::string("���݂��Ȃ��A�j���[�V�������w�肳��܂����B(") + anim_name + ")", "GraphAnimatorComponent::StartAnimation");
				return false; 
			}
			else {
				current_animation_ = it->second;
				current_animation_name_ = anim_name;
				current_frame_ = 0;
				frame_counter_ = 0;
				is_playing_ = true;
				roop_flag_ = roop_flag;
				return true;
			}

		}

		void GraphAnimatorComponent::StopAnimation()
		{
			is_playing_ = false;
		}

		void GraphAnimatorComponent::SetCurrentFrameDrawAreaToDrawGraphComponent_()
		{
			draw_graph_component_->draw_area(std::get<1>((*current_animation_)[current_frame_]));
			draw_graph_component_->scale(std::get<2>((*current_animation_)[current_frame_]));
			draw_graph_component_->rotation_rad(std::get<3>((*current_animation_)[current_frame_]));
		}

	}
}
