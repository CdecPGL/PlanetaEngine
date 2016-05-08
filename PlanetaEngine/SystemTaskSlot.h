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
			EventUpdateConvertCoordinatePhase = 6,
			CameraUpdateConvertCoordinatePhase = 7,
			PostCameraUpdateConvertCoordinatePhase = 8,
			GUIUpdateEarlyConvertCoordinatePhase = 9,
			GUIUpdateLateConvertCoordinatePhase = 10,
			PreDrawUpdateConvertCoordinatePhase = 11,
			DrawPhase = 12,
		};
		constexpr int SYSTEM_TASK_SLOT_SIZE = 13;
	}
}