#pragma once

namespace plnt {
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_bgm_controller {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_bgm_controller() = 0 {}

		virtual bool start(bool loop_flag) = 0;
		virtual bool stop() = 0;
		virtual bool fade_in(int frame, bool loop_flag) = 0;
		virtual bool fade_out(int frame) = 0;
		[[nodiscard]] virtual bool is_valid() const = 0;
		[[nodiscard]] virtual bool is_playing() const = 0;
	};
}
