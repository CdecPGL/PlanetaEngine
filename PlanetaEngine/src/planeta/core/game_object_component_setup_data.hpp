#pragma once

#include "weak_pointer.hpp"

namespace plnt {
	class i_scene_internal;
	class i_game_object;

	namespace private_ {
		struct game_object_component_set_up_data {
			weak_pointer<i_game_object> holder_game_object;
			weak_pointer<i_scene_internal> scene_internal_interface;
		};
	}
}
