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
				debug::SystemLog::instance().LogError("DrawGraphComponentを取得できませんでした。", "GraphAnimatorComponent::Initialize_");
				return false;
			}
			
		}

		void planeta_engine::components::GraphAnimatorComponent::Update_()
		{
			if (is_playing_) {
				if (current_animation_ == nullptr) { is_playing_ = false; }
				else {
					//フレームカウントが0なら、描画コンポーネントを更新する
					if (frame_counter_ == 0) { SetCurrentFrameDrawAreaToDrawGraphComponent_(); }
					++frame_counter_;
					//フレームカウントが、現在のフレームの表示フレーム以上だったら次に
					if (frame_counter_ >= std::get<0>((*current_animation_)[current_frame_])) {
						++current_frame_;
						frame_counter_ = 0;
						//新しいフレームが現在のアニメーションのフレーム数を超えていたら
						if (current_frame_ >= (*current_animation_).size()) {
							if (roop_flag_) { //ループフラグが立っているときはフレームを0に戻す
								current_frame_ = 0;
							}
							else { //立っていないときは再生を停止する
								is_playing_ = false;
							}
						}
						else { //なにもなく新しいフレームに移行した
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
				debug::SystemLog::instance().LogError(std::string("存在しないアニメーションが指定されました。(") + anim_name + ")", "GraphAnimatorComponent::StartAnimation");
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
