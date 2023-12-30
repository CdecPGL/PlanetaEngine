#pragma once

#include "non_owing_pointer.hpp"

namespace plnt {
	class ISceneInternal;
	class i_game_object;

	namespace private_ {
		struct game_object_component_set_up_data {
			non_owing_pointer<i_game_object> holder_game_object;
			WeakPointer<i_scene_internal> scene_internal_interface;
		};
	}
}
