#pragma once

#include "Vector2D.hpp"

namespace plnt {
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_draw_system {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_draw_system() = 0 { }
		/*! スクリーン座標をGameObject座空間標に変換*/
		[[nodiscard]] virtual vector_2dd covert_position_screen_space_to_game_object_space(
			const vector_2di &gui_space_pos) const = 0;
		/*! GameObject座標をスクリーン座標に変換*/
		[[nodiscard]] virtual vector_2di covert_position_game_object_space_to_screen_space(
			const vector_2dd &game_object_space_pos) const = 0;
	};
}
