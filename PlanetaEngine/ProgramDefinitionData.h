﻿#pragma once

#include <string>
#include "CollisionGroupMatrix.h"

namespace planeta {
	namespace private_ {
		struct ProgramDefinitionData {
			bool load_from_file;
			std::string startup_scene_id;
			CollisionGroupMatrix collision_group_matrix;
		};
	}
}