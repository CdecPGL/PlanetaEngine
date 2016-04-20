#pragma once

#include "AccessorTemplate.h"
#include "SAImplGameObjectCreateFunctions.h"
#include "SAImplTaskCreateFunctions.h"
#include "SAImplTaskGetFunctions.h"
#include "SAImplCameraFunctions.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class SceneAccessorForGameObject final : public utility::AccessorTemplate<
		core::ScenePublicInterface,
		scene_accessor_impl::SAImplGameObjectCreateFunctions,
		scene_accessor_impl::SAImplTaskCreateFunctions,
		scene_accessor_impl::SAImplTaskGetFunctions,
		scene_accessor_impl::SAImplCameraFunctions
	> {
	public:using AccessorTemplate::AccessorTemplate;
	};
}