#include "../core/screen_drawer_2d.hpp"
#include "../core/i_game_object.hpp"
#include "../core/matrix_22.hpp"
#include "CTransform2D.hpp"
#include "CDrawLine2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_line_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_line_2d, width)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_line_2d, length)
			.shallow_copy_target(&c_draw_line_2d::width_)
			.shallow_copy_target(&c_draw_line_2d::length_)
			.shallow_copy_target(&c_draw_line_2d::wire_positions_);
	}

	c_draw_line_2d::c_draw_line_2d() : wire_positions_(2), width_(1), length_(0) { }

	void c_draw_line_2d::draw_proc(screen_drawer_2d &drawer) {
		//トランスフォームからワイヤーの位置を更新
		wire_positions_[0] = static_cast<vector_2df>(get_draw_center_position());
		wire_positions_[1] = wire_positions_[0] + static_cast<vector_2df>(math::rotation_transform(
			get_draw_rotation_red(), vector_2dd(1.0, 0.0)) * length() * get_draw_scale().x);
		//描画
		drawer.draw_wire(wire_positions_, width_ * get_draw_scale().y, color());
	}
}
