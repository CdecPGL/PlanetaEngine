#pragma once

#include <memory>

namespace planeta_engine {
	class Camera;
	namespace core {
		struct SceneDataForGameProcess {
			std::shared_ptr<Camera> camera;
		};
	}
}