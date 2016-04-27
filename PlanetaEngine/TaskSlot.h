#pragma once

namespace planeta_engine {
	enum class TaskSlot : unsigned int{
		PreCollisionEarlyPhase = 0,
		PreCollisionLatePhase = 1,
		PostCollisionEarlyPhase = 2,
		PostCollisionLatePhase = 3,
		EventUpdateEarlyPhase = 4,
		EventUpdateLatePhase = 5,
		GUIUpdateEarlyPhase = 6,
		GUIUpdateLatePhase = 7,
	};
	constexpr int TASK_SLOT_SIZE = 8;
}
