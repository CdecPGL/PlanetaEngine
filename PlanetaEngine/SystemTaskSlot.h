#pragma once

namespace planeta {
	namespace core {
		enum class SystemTaskSlot {
			ApplyVelocityPhase = 0,
			CollitionDetectPhase = 1,
			ApplyCameraPhase = 2,
			DrawPhase = 3,
		};
		constexpr int SYSTEM_TASK_SLOT_SIZE = 4;
	}
}