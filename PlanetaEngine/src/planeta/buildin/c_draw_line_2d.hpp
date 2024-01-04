#pragma once

#include <vector>

#include "../core/vector_2d.hpp"

#include "c_draw_2d.hpp"

namespace plnt {
	/*! 平面上直線描画コンポーネント*/
	class c_draw_line_2d final : public c_draw_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_line_2d);

	public:
		using super = c_draw_2d;
		c_draw_line_2d();
		/*アクセサ*/
		/*! 線の幅を取得*/
		[[nodiscard]] int width() const { return width_; }
		/*! 線の幅を設定*/
		c_draw_line_2d &width(const int w) {
			width_ = w;
			return *this;
		}

		/*! 線の長さを取得*/
		[[nodiscard]] double length() const { return length_; }
		/*! 線の長さを設定*/
		c_draw_line_2d &length(const double l) {
			length_ = l;
			return *this;
		}

	private:
		std::vector<vector_2df> wire_positions_;
		int width_;
		double length_;
		/*描画処理*/
		void draw_proc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_line_2d);
}
