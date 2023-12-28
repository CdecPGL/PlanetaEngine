#pragma once

namespace plnt {
	class i_gm_controller {
	public:
		i_gm_controller() = default;
		i_gm_controller(const i_gm_controller &) = default;
		i_gm_controller(i_gm_controller &&) = default;
		virtual ~i_gm_controller() = default;
		i_gm_controller &operator=(const i_gm_controller &) = default;
		i_gm_controller &operator=(i_gm_controller &&) = default;

		virtual bool start(bool loop_flag) = 0;
		virtual bool stop() = 0;
		virtual bool fade_in(int frame, bool loop_flag) = 0;
		virtual bool fade_out(int frame) = 0;
		[[nodiscard]] virtual bool is_valid() const = 0;
		[[nodiscard]] virtual bool is_playing() const = 0;
	};
}
