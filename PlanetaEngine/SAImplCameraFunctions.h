#pragma once

namespace planeta_engine {
	class Camera;
	namespace core {
		class ScenePublicInterface;
	}
	namespace scene_accessor_impl {
		class SAImplCameraFunctions {
		public:
			explicit SAImplCameraFunctions(core::ScenePublicInterface& spi);
			Camera& camera() { return camera_; }
		private:
			Camera& camera_;
		};
	}
}