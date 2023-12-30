#pragma once

#include <memory>

#include "i_bgm_controller.hpp"
#include "i_sound_effect_controller.hpp"

namespace plnt {
	class resource_base;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_sound_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_sound_manager() = 0 { }

		virtual std::shared_ptr<i_bgm_controller> get_bgm_controller(const std::shared_ptr<resource_base> &music_resource)
		=0;
		virtual std::shared_ptr<i_sound_effect_controller> get_sound_effect_controller(
			const std::shared_ptr<resource_base> &sound_resource) = 0;
	};
}
