#pragma once

#include <unordered_map>

#include "transform_system.hpp"

namespace plnt::private_ {
	class standard_transform_system final : public transform_system {
	public:
		bool initialize() override;
		void update() override;
		void finalize() override;

		void apply_velocity() override;

		int register_transform_2d(c_transform_2d *transform2d) override;
		bool remove_transform_2d(int id) override;
		int register_transform_gui(c_transform_gui *transform_gui) override;
		bool remove_transform_gui(int id) override;

	private:
		std::unordered_map<int, c_transform_2d *> transform2d_map_;
		std::unordered_map<int, c_transform_gui *> transform_gui_map_;
		int id_counter_2d_ = 0;
		int id_counter_gui_ = 0;
	};
}
