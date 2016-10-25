#pragma once

#include <vector>
#include <memory>
#include "ISoundEffectController.h"

namespace planeta {
	class RSound;
	class SoundEffectController final : public ISoundEffectController {
	public:
		SoundEffectController(const std::shared_ptr<RSound>& mr) :sound_resource_(mr) {}
		~SoundEffectController();
		bool Play()override;
		bool is_valid()const override;

		void Dispose();
	private:
		std::shared_ptr<RSound> sound_resource_;
		std::vector<int> duplicated_dx_handles_;
	};
}
