#pragma once

#include "NonOwingPointer.hpp"

namespace plnt {
	class ISceneInternal;
	class i_game_object;

	namespace private_ {
		struct game_object_component_set_up_data {
			NonOwingPointer<i_game_object> holder_game_object;
			WeakPointer<ISceneInternal> scene_internal_interface;
		};
	}
}
