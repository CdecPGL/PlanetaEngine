#pragma once

#include <vector>
#include <memory>
#include "ISoundEffectController.h"

namespace planeta_engine {
	namespace resources {
		class SoundResource;
	}
	namespace core {
		class SoundEffectController final: public ISoundEffectController{
		public:
			SoundEffectController(const std::shared_ptr<resources::SoundResource>& mr) :sound_resource_(mr) {}
			~SoundEffectController();
			bool Play()override;
			bool is_valid()const override;

			void Dispose();
		private:
			std::shared_ptr<resources::SoundResource> sound_resource_;
			std::vector<int> duplicated_dx_handles_;
		};
	}
}