#pragma once

#include <memory>
#include "i_bgm_controller.hpp"

namespace plnt {
	class r_music;

	class bgm_controller final : public i_bgm_controller {
	public:
		bgm_controller() = delete;
		bgm_controller(const bgm_controller &) = delete;
		bgm_controller(bgm_controller &&) = delete;
		bgm_controller &operator=(const bgm_controller &) = delete;
		bgm_controller &operator=(bgm_controller &&) = delete;

		explicit bgm_controller(std::shared_ptr<r_music> mr) : music_resource_(std::move(mr)) { }

		~bgm_controller() override;
		bool start(bool loop_flag) override;
		bool stop() override;
		bool fade_in(int frame, bool loop_flag) override;
		bool fade_out(int frame) override;
		[[nodiscard]] bool is_valid() const override;
		[[nodiscard]] bool is_playing() const override;

		void update();
		void dispose();
		[[nodiscard]] std::shared_ptr<r_music> resource() const { return music_resource_; }

	private:
		std::shared_ptr<r_music> music_resource_;

		enum class state { fade_in, fade_out, none };

		state state_ = state::none;
		int fade_total_frame_ = 0;
		int fade_frame_counter_ = 0;

		[[nodiscard]] bool play(bool loop_flag) const;
	};
}
