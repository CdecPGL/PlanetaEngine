#pragma once

#include "sound_manager.hpp"

namespace plnt::private_ {
	class standard_sound_manager final : public sound_manager {
	public:
		standard_sound_manager();
		standard_sound_manager(const standard_sound_manager &) = delete;
		standard_sound_manager(standard_sound_manager &&) = delete;
		~standard_sound_manager() override;
		standard_sound_manager &operator=(const standard_sound_manager &) = delete;
		standard_sound_manager &operator=(standard_sound_manager &&) = delete;

		bool initialize() override;
		void finalize() override;
		void update() override;

		[[nodiscard]] std::shared_ptr<i_bgm_controller> get_bgm_controller(
			const std::shared_ptr<resource_base> &music_resource) override;
		[[nodiscard]] std::shared_ptr<i_sound_effect_controller> get_sound_effect_controller(
			const std::shared_ptr<resource_base> &sound_resource) override;
		void reset() override;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
	};
}
