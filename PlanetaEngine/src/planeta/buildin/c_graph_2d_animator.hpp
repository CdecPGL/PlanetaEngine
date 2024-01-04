#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>

#include "../core/game_object_standard_component.hpp"
#include "planeta/core/rectangle.hpp"

namespace plnt {
	class c_draw_graph_2d;

	/*! 平面画像アニメータコンポーネント*/
	class c_graph_2d_animator final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_graph_2d_animator);

	public:
		using super = game_object_standard_component;
		/*! フレームデータのタイプ<表示フレーム数,表示範囲,拡大度,回転度(ラジアン)>*/
		using frame_data_type = std::tuple<unsigned int, rectangle<int>, vector_2dd, double>;
		/*! 単純化フレームデータのタイプ<表示フレーム数,表示範囲>*/
		using simple_frame_data_type = std::pair<unsigned int, rectangle<int>>;
		/*! アニメーションセット(フル)*/
		void set_animation(const std::string &anim_name, const std::vector<frame_data_type> &frames);
		/*! アニメーションセット(シンプル)*/
		void set_animation(const std::string &anim_name, const std::vector<simple_frame_data_type> &frames);
		/*! 指定名のアニメーションの再生を開始する*/
		bool start_animation(const std::string &anim_name, bool loop_flag);
		/*! アニメーションが再生中なら停止する*/
		void stop_animation();
		/*! 再生中のアニメーション名を取得する*/
		[[nodiscard]] std::string get_playing_animation_name() const { return is_playing_ ? current_animation_name_ : ""; }

		//アクセサ
		/*! 全アニメーションデータの設定*/
		void animation_data(const std::unordered_map<std::string, std::vector<frame_data_type>> &data);

	private:
		weak_pointer<c_draw_graph_2d> draw_graph_component_ = {};
		unsigned int frame_counter_ = 0;
		bool is_playing_ = false;
		bool loop_flag_ = false;
		unsigned int current_frame_ = 0;
		std::unordered_map<std::string, std::vector<frame_data_type>> animations_;
		std::vector<frame_data_type> *current_animation_ = nullptr;
		std::string current_animation_name_;
		void set_current_frame_draw_area_to_draw_graph_component() const;
		[[nodiscard]] bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_initialized() override;
		void update_animation();
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_graph_2d_animator);
}
