#pragma once

namespace planeta_engine {
	namespace core {
		enum class SystemTaskSlot {
			ApplyVelocityPhase = 0,
			CollitionDetectPhase = 1,
			DrawPhase = 2,
		};
		constexpr int SYSTEM_TASK_SLOT_SIZE = 3;
	}
}