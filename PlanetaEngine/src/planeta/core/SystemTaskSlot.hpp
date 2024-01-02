#pragma once

namespace plnt::private_ {
	enum class system_task_slot {
		apply_velocity_phase = 0,
		collision_detect_phase = 1,
		apply_camera_phase = 2,
		draw_phase = 3,
		draw_gui_phase = 4,
	};

	constexpr int system_task_slot_size = 5;
}
