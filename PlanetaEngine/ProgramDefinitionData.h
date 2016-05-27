#pragma once

#include <string>
#include "CollisionGroupMatrix.h"

namespace planeta_engine {
	namespace core {
		struct ProgramDefinitionData {
			bool load_from_file;
			std::string startup_scene_id;
			CollisionGroupMatrix collision_group_matrix;
		};
	}
}