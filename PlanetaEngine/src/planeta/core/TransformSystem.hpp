﻿#pragma once

#include "i_transform_system.hpp"
#include "scene_module.hpp"

namespace plnt {
	class CTransform2D;
	class CTransformGUI;

	namespace private_ {
		class transform_system : public i_transform_system, public scene_module {
		public:
			transform_system() = default;
			transform_system(const transform_system &) = delete;
			transform_system(transform_system &&) = delete;
			~transform_system() override = default;
			transform_system &operator=(const transform_system &) = delete;
			transform_system &operator=(transform_system &&) = delete;

			virtual void apply_velocity() = 0;
			virtual int register_transform_2d(CTransform2D *transform2d) = 0;
			virtual bool remove_transform_2d(int id) = 0;
			virtual int register_transform_gui(CTransformGUI *transform_gui) = 0;
			virtual bool remove_transform_gui(int id) = 0;
		};
	}
}
