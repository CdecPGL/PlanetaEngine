#pragma once

namespace planeta_engine {
	namespace core {
		enum class SystemTaskSlot {
			PreCollisionEarlyConvertCoordinatePhase = 0,
			PreCollisionLateConvertCoordinatePhase = 1,
			ApplyVelocityPhase = 2,
			CollitionDetectPhase = 3,
			PostCollisionEarlyConvertCoordinatePhase = 4,
			PostCollisionLateConvertCoordinatePhase = 5,
			EventUpdateEarlyConvertCoordinatePhase = 6,
			EventUpdateLateConvertCoordinatePhase = 7,
			UpdateAnimationPhase = 8,
			GUIUpdateEarlyConvertCoordinatePhase = 9,
			GUIUpdateLateConvertCoordinatePhase = 10,
			DrawPhase = 11,
		};
		constexpr int SYSTEM_TASK_SLOT_SIZE = 12;
	}
}