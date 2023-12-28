#pragma once

#include "NonOwingPointer.hpp"

namespace plnt {
	class ISceneInternal;
	class IGameObject;

	namespace private_ {
		struct game_object_component_set_up_data {
			NonOwingPointer<IGameObject> holder_game_object;
			WeakPointer<ISceneInternal> scene_internal_interface;
		};
	}
}
