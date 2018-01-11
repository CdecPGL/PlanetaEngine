#pragma once

#include "planeta/core/ConfigManager.hpp"

namespace plnt {
	namespace private_ {
		class StandardConfigManager final: public ConfigManager{
		public:
			virtual bool LoadSystemConfig(const File& file) override;
			virtual bool LoadUserConfig(const File& file) override;
			virtual int color_bit_depth() const override;
			virtual std::string startup_scene_id() const override;
			virtual std::string game_title() const override;
			virtual std::string game_version_string() const override;
			virtual std::array<int, 3> game_version_numbers() const override;
			virtual bool is_window_mode() const override;
			virtual Vector2Di draw_size() const override;
			virtual Vector2Di window_size() const override;
			virtual const CollisionGroupMatrix& collision_group_matrix() const override;
			virtual bool is_cursor_visible()const override;
		private:
			int color_bit_depth_ = -1;
			std::string startup_scene_id_ = "NULL";
			std::string game_title_ = "NULL";
			std::string game_version_string_ = "NULL";
			std::array<int, 3> game_version_numbers_ = { -1,-1,-1 };
			bool is_window_mode_ = false;
			Vector2Di draw_size_ = { -1,-1 };
			Vector2Di window_size_ = { -1,-1 };
			CollisionGroupMatrix collision_group_matrix_;
			bool is_cursor_visible_ = true;
		};
	}
}