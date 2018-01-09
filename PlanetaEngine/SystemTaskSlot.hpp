#pragma once

namespace planeta {
	namespace private_ {
		enum class SystemTaskSlot {
			ApplyVelocityPhase = 0,
			CollitionDetectPhase = 1,
			ApplyCameraPhase = 2,
			DrawPhase = 3,
			DrawGUIPhase = 4,
		};
		constexpr int SYSTEM_TASK_SLOT_SIZE = 5;
	}
}