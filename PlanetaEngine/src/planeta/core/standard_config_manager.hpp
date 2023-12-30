#pragma once

#include "config_manager.hpp"

namespace plnt::private_ {
	class standard_config_manager final : public config_manager {
	public:
		bool load_system_config(const file &file) override;
		bool load_user_config(const file &file) override;
		[[nodiscard]] int color_bit_depth() const override;
		[[nodiscard]] std::string startup_scene_id() const override;
		[[nodiscard]] std::string game_title() const override;
		[[nodiscard]] std::string game_version_string() const override;
		[[nodiscard]] std::array<int, 3> game_version_numbers() const override;
		[[nodiscard]] bool is_window_mode() const override;
		[[nodiscard]] vector_2di draw_size() const override;
		[[nodiscard]] vector_2di window_size() const override;
		[[nodiscard]] const plnt::collision_group_matrix &collision_group_matrix() const override;
		[[nodiscard]] bool is_cursor_visible() const override;

	private:
		int color_bit_depth_ = -1;
		std::string startup_scene_id_ = "NULL";
		std::string game_title_ = "NULL";
		std::string game_version_string_ = "NULL";
		std::array<int, 3> game_version_numbers_ = {-1, -1, -1};
		bool is_window_mode_ = false;
		vector_2di draw_size_ = {-1, -1};
		vector_2di window_size_ = {-1, -1};
		plnt::collision_group_matrix collision_group_matrix_;
		bool is_cursor_visible_ = true;
	};
}
