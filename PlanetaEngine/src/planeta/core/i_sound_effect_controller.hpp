#pragma once

namespace plnt {
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_sound_effect_controller {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_sound_effect_controller() = 0 {}

		virtual bool play() = 0;
		virtual bool is_valid() const = 0;
	};
}
