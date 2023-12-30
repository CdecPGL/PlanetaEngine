#pragma once

#include <vector>
#include <memory>

#include "i_sound_effect_controller.hpp"

namespace plnt {
	class RSound;

	class sound_effect_controller final : public i_sound_effect_controller {
	public:
		sound_effect_controller(const sound_effect_controller &) = delete;
		sound_effect_controller(sound_effect_controller &&) = default;
		explicit sound_effect_controller(std::shared_ptr<RSound> mr) : sound_resource_(std::move(mr)) {}
		sound_effect_controller &operator=(const sound_effect_controller &) = delete;
		sound_effect_controller &operator=(sound_effect_controller &&) = default;

		~sound_effect_controller() override;
		bool play() override;
		[[nodiscard]] bool is_valid() const override;

		void dispose();

	private:
		std::shared_ptr<RSound> sound_resource_;
		std::vector<int> duplicated_dx_handles_;
	};
}
