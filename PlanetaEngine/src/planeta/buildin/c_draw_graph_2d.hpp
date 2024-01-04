#pragma once

#include <memory>
#include <vector>

#include "planeta/core/rectangle.hpp"
#include "c_draw_2d.hpp"

namespace plnt {
	namespace private_ {
		class graph_draw_data_2d;
	}

	/*! 平面画像描画コンポーネント*/
	class c_draw_graph_2d final : public c_draw_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_graph_2d);

	public:
		using super = c_draw_2d;
		c_draw_graph_2d();
		~c_draw_graph_2d() override;

		/*! 画像リソースをセット*/
		bool set_graph_resource(const std::string &resource_id);
		/*アクセサ*/
		/*! 画像リソースをIDでセット*/
		c_draw_graph_2d &graph_resource_id(const std::string &res_id);
		/*! 画像上の描画領域をピクセル単位で取得*/
		[[nodiscard]] const rectangle<int> &draw_area() const { return draw_area_; }
		/*! 画像上の描画領域をピクセル単位で設定*/
		c_draw_graph_2d &draw_area(const rectangle<int> &rect) {
			draw_area_ = rect;
			update_uv_position();
			return *this;
		}

		/*! 画像を反転させるか取得*/
		[[nodiscard]] bool reverse() const { return reverse_; }
		/*! 画像を反転させるか設定*/
		c_draw_graph_2d &reverse(const bool rev) {
			reverse_ = rev;
			update_uv_position();
			return *this;
		}

		/*! 画像の中心を[0.0,1.0]*[0.0,1.0]の範囲で取得*/
		[[nodiscard]] const vector_2dd &graph_center() const { return graph_center_; }
		/*! 画像の中心を[0.0,1.0]*[0.0,1.0]の範囲で設定*/
		void graph_center(const vector_2dd &pos) { graph_center_ = pos; }

	private:
		/*反転描画フラグ*/
		bool reverse_ = false;
		/*画像上で表示する範囲*/
		rectangle<int> draw_area_;
		/*画像の中心位置*/
		vector_2dd graph_center_ = vector_2dd(0.5, 0.5);
		/*画像描画データ*/
		std::shared_ptr<private_::graph_draw_data_2d> graph_draw_data_;
		/*ポリゴン情報更新*/
		void update_polygon() const;
		/*UV座標更新*/
		void update_uv_position() const;
		/*描画処理*/
		void draw_proc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_graph_2d);
}
