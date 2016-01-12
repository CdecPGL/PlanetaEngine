#pragma once

#include "AccessorTemplate.h"
#include "SAImplGameObjectCreateFunctions.h"
#include "SAImplGameProcessCreateFunctions.h"
#include "SAImplGameProcessGetFunctions.h"
#include "SAImplCameraFunctions.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class SceneAccessorForGameObject final : public utility::AccessorTemplate<
		core::ScenePublicInterface,
		scene_accessor_impl::SAImplGameObjectCreateFunctions,
		scene_accessor_impl::SAImplGameProcessCreateFunctions,
		scene_accessor_impl::SAImplGameProcessGetFunctions,
		scene_accessor_impl::SAImplCameraFunctions
	> {
	public:using AccessorTemplate::AccessorTemplate;
	};
}