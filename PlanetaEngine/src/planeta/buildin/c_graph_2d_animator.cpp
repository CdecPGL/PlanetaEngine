#include "../core/i_game_object.hpp"
#include "../core/log_utility.hpp"
#include "c_draw_graph_2d.hpp"
#include "c_graph_2d_animator.hpp"
#include "TInstant.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_graph_2d_animator) {
		registerer
			.write_only_property("animation_data", &c_graph_2d_animator::animation_data)
			.shallow_copy_target(&c_graph_2d_animator::animations_);
	}

	bool c_graph_2d_animator::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		draw_graph_component_.reset(com_getter.get_component<c_draw_graph_2d>());
		if (draw_graph_component_ == nullptr) {
			PE_LOG_ERROR("DrawGraphComponentを取得できませんでした。");
			return false;
		}
		return true;
	}

	void c_graph_2d_animator::on_initialized() {
		super::on_initialized();
		//アニメーションの更新タスクをゲームオブジェクトにアタッチ
		const auto tsk = game_object().create_and_attach_task<t_instant>(task_slot::pre_draw_update_phase);
		tsk->set_execute_function([this]() { update_animation(); });
	}

	void c_graph_2d_animator::update_animation() {
		if (is_playing_) {
			if (current_animation_ == nullptr) { is_playing_ = false; } else {
				//フレームカウントが0なら、描画コンポーネントを更新する
				if (frame_counter_ == 0) { set_current_frame_draw_area_to_draw_graph_component(); }
				++frame_counter_;
				//フレームカウントが、現在のフレームの表示フレーム以上だったら次に
				if (frame_counter_ >= std::get<0>((*current_animation_)[current_frame_])) {
					++current_frame_;
					frame_counter_ = 0;
					//新しいフレームが現在のアニメーションのフレーム数を超えていたら
					if (current_frame_ >= (*current_animation_).size()) {
						if (loop_flag_) {
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

	void c_graph_2d_animator::set_animation(const std::string &anim_name, const std::vector<frame_data_type> &frames) {
		animations_[anim_name] = frames;
	}

	void c_graph_2d_animator::set_animation(const std::string &anim_name,
	                                     const std::vector<simple_frame_data_type> &frames) {
		std::vector<frame_data_type> new_frames;
		new_frames.resize(frames.size());
		for (unsigned int i = 0; i < frames.size(); ++i) {
			new_frames[i] = frame_data_type(frames[i].first, frames[i].second, vector_2dd(1.0, 1.0), 0.0);
		}
		animations_[anim_name] = new_frames;
	}

	bool c_graph_2d_animator::start_animation(const std::string &anim_name, const bool loop_flag) {
		if (const auto it = animations_.find(anim_name); it == animations_.end()) {
			PE_LOG_ERROR("存在しないアニメーションが指定されました。(", anim_name, ")");
			return false;
		} else {
			current_animation_ = &(it->second);
			current_animation_name_ = anim_name;
			current_frame_ = 0;
			frame_counter_ = 0;
			is_playing_ = true;
			loop_flag_ = loop_flag;
			return true;
		}
	}

	void c_graph_2d_animator::stop_animation() { is_playing_ = false; }

	void c_graph_2d_animator::animation_data(const std::unordered_map<std::string, std::vector<frame_data_type>> &data) {
		animations_ = data;
	}

	void c_graph_2d_animator::set_current_frame_draw_area_to_draw_graph_component() const {
		draw_graph_component_->draw_area(std::get<1>((*current_animation_)[current_frame_]));
		draw_graph_component_->relative_scale(std::get<2>((*current_animation_)[current_frame_]));
		draw_graph_component_->relative_rotation_rad(std::get<3>((*current_animation_)[current_frame_]));
	}
}
