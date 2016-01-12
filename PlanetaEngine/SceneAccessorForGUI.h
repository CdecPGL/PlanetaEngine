#pragma once

#include "AccessorTemplate.h"
#include "SAImplGUIOperateFunctions.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	class SceneAccessorForGUI final : public utility::AccessorTemplate<
		core::ScenePublicInterface,
		scene_accessor_impl::SAImplGUIOperateFunctions
	> {
	public:using AccessorTemplate::AccessorTemplate;
	};
}