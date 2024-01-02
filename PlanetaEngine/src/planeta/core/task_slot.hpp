#pragma once

namespace plnt {
	enum class task_slot : unsigned int {
		player_input_update_phase = 0,
		pre_collision_early_phase = 1,
		pre_collision_late_phase = 2,
		post_collision_early_phase = 3,
		post_collision_late_phase = 4,
		event_update_phase = 5,
		camera_update_phase = 6,
		post_camera_update_phase = 7,
		gui_update_early_phase = 8,
		gui_update_late_phase = 9,
		pre_draw_update_phase = 10,
	};

	constexpr int task_slot_size = 11;
}
