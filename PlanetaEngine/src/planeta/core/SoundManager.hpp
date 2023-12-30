#pragma once

#include "i_sound_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt::private_ {
	class sound_manager : public i_sound_manager, public SubSystemManager {
	public:
		sound_manager() = default;
		sound_manager(const sound_manager &) = delete;
		sound_manager(sound_manager &&) = delete;
		~sound_manager() override = default;
		sound_manager &operator=(const sound_manager &) = delete;
		sound_manager &operator=(sound_manager &&) = delete;

		virtual bool initialize() = 0;
		virtual void finalize() = 0;
		virtual void update() = 0;
		virtual void reset() = 0;
	};
}
