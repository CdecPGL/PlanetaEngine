#include "..\core\i_game_object.hpp"
#include "..\core\log_utility.hpp"
#include "..\core\log_utility.hpp"

#include "CDrawGraph2D.hpp"
#include "CGraph2DAnimator.hpp"
#include "TInstant.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CGraph2DAnimator) {
		registerer
			.write_only_property("animation_data", &CGraph2DAnimator::animation_data)
			.shallow_copy_target(&CGraph2DAnimator::animations_);
	}

	bool CGraph2DAnimator::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		draw_graph_component_.reset(com_getter.get_component<CDrawGraph2D>());
		if (draw_graph_component_ == nullptr) {
			PE_LOG_ERROR("DrawGraphComponentを取得できませんでした。");
			return false;
		}
		return true;
	}

	void CGraph2DAnimator::on_initialized() {
		super::on_initialized();
		//アニメーションの更新タスクをゲームオブジェクトにアタッチ
		auto tsk = game_object().create_and_attach_task<TInstant>(task_slot::pre_draw_update_phase);
		tsk->SetExcuteFunction([this]() { UpdateAnimation(); });
	}

	void CGraph2DAnimator::UpdateAnimation() {
		if (is_playing_) {
			if (current_animation_ == nullptr) { is_playing_ = false; } else {
				//フレームカウントが0なら、描画コンポーネントを更新する
				if (frame_counter_ == 0) { SetCurrentFrameDrawAreaToDrawGraphComponent_(); }
				++frame_counter_;
				//フレームカウントが、現在のフレームの表示フレーム以上だったら次に
				if (frame_counter_ >= std::get<0>((*current_animation_)[current_frame_])) {
					++current_frame_;
					frame_counter_ = 0;
					//新しいフレームが現在のアニメーションのフレーム数を超えていたら
					if (current_frame_ >= (*current_animation_).size()) {
						if (roop_flag_) {
							//ループフラグが立っているときはフレームを0に戻す
							current_frame_ = 0;
						} else {
							//立っていないときは再生を停止する
							is_playing_ = false;
						}
					} else {
						//なにもなく新しいフレームに移行した
					}
				}
			}
		}
	}

	void CGraph2DAnimator::SetAnimation(const std::string &anim_name, const std::vector<FrameDataType> &frames) {
		animations_[anim_name] = frames;
	}

	void CGraph2DAnimator::SetAnimation(const std::string &anim_name, const std::vector<SimpleFrameDataType> &frames) {
		std::vector<FrameDataType> new_frames;
		new_frames.resize(frames.size());
		for (unsigned int i = 0; i < frames.size(); ++i) {
			new_frames[i] = FrameDataType(frames[i].first, frames[i].second, vector_2dd(1.0, 1.0), 0.0);
		}
		animations_[anim_name] = new_frames;
	}

	bool CGraph2DAnimator::StartAnimation(const std::string &anim_name, bool roop_flag) {
		auto it = animations_.find(anim_name);
		if (it == animations_.end()) {
			PE_LOG_ERROR("存在しないアニメーションが指定されました。(", anim_name, ")");
			return false;
		} else {
			current_animation_ = &(it->second);
			current_animation_name_ = anim_name;
			current_frame_ = 0;
			frame_counter_ = 0;
			is_playing_ = true;
			roop_flag_ = roop_flag;
			return true;
		}
	}

	void CGraph2DAnimator::StopAnimation() { is_playing_ = false; }

	void CGraph2DAnimator::animation_data(const std::unordered_map<std::string, std::vector<FrameDataType>> &data) {
		animations_ = data;
	}

	void CGraph2DAnimator::SetCurrentFrameDrawAreaToDrawGraphComponent_() {
		draw_graph_component_->draw_area(std::get<1>((*current_animation_)[current_frame_]));
		draw_graph_component_->relative_scale(std::get<2>((*current_animation_)[current_frame_]));
		draw_graph_component_->relative_rotation_rad(std::get<3>((*current_animation_)[current_frame_]));
	}
}
