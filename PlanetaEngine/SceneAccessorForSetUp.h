#pragma once

#include "AccessorTemplate.h"
#include "SAImplGameObjectCreateFunctions.h"
#include "SAImplTaskCreateFunctions.h"
#include "SAImplTaskKeyPositionFunctions.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class SceneAccessorForSetUp final : public utility::AccessorTemplate<
		core::ScenePublicInterface,
		scene_accessor_impl::SAImplGameObjectCreateFunctions,
		scene_accessor_impl::SAImplTaskCreateFunctions,
		scene_accessor_impl::SAImplTaskKeyPositionFunctions
	> {
	public:using AccessorTemplate::AccessorTemplate;
	};
}
