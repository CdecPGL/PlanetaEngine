#pragma once

#include "AccessorTemplate.h"
#include "SAImplGameObjectCreateFunctions.h"
#include "SAImplGameObjectManagerSetUpFunctions.h"
#include "SAImplGUIOperateFunctions.h"
#include "SAImplGameProcessCreateFunctions.h"
#include "SAImplGameProcessKeyPositionFunctions.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class SceneAccessorForSetUp final : public utility::AccessorTemplate<
		core::ScenePublicInterface,
		scene_accessor_impl::SAImplGameObjectCreateFunctions,
		scene_accessor_impl::SAImplGameObjectManagerSetUpFunctions,
		scene_accessor_impl::SAImplGUIOperateFunctions,
		scene_accessor_impl::SAImplGameProcessCreateFunctions,
		scene_accessor_impl::SAImplGameProcessKeyPositionFunctions
	> {
	public:using AccessorTemplate::AccessorTemplate;
	};
}
