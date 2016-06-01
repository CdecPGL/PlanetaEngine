#pragma once

namespace planeta {
	enum class TaskSlot : unsigned int{
		PreCollisionEarlyPhase = 0,
		PreCollisionLatePhase = 1,
		PostCollisionEarlyPhase = 2,
		PostCollisionLatePhase = 3,
		EventUpdatePhase = 4,
		CameraUpdatePhase = 5,
		PostCameraUpdatePhase = 6,
		GUIUpdateEarlyPhase = 7,
		GUIUpdateLatePhase = 8,
		PreDrawUpdatePhase = 9,
	};
	constexpr int TASK_SLOT_SIZE = 10;
}
