#pragma once

namespace planeta {
	enum class TaskSlot : unsigned int{
		PlayerInputUpdatePhase = 0,
		PreCollisionEarlyPhase = 1,
		PreCollisionLatePhase = 2,
		PostCollisionEarlyPhase = 3,
		PostCollisionLatePhase = 4,
		EventUpdatePhase = 5,
		CameraUpdatePhase = 6,
		PostCameraUpdatePhase = 7,
		GUIUpdateEarlyPhase = 8,
		GUIUpdateLatePhase = 9,
		PreDrawUpdatePhase = 10,
	};
	constexpr int TASK_SLOT_SIZE = 11;
}
