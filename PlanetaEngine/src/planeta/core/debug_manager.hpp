﻿#pragma once

#include "i_debug_manager.hpp"
#include "sub_system_manager.hpp"

namespace plnt::private_ {
	class rendering_manager;

	class debug_manager : public i_debug_manager, public sub_system_manager {
	public:
		debug_manager() = default;
		debug_manager(const debug_manager &) = delete;
		debug_manager(debug_manager &&) = delete;
		~debug_manager() override = default;
		debug_manager &operator=(const debug_manager &) = delete;
		debug_manager &operator=(debug_manager &&) = delete;

		virtual bool initialize(rendering_manager &rendering_mgr) = 0;
		virtual void finalize() = 0;
		virtual void pre_rendering_update() = 0;
		virtual void post_rendering_update() = 0;
	};
}
