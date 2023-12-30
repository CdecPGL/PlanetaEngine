#pragma once

#include "SoundManager.hpp"

namespace plnt {
	namespace private_ {
		class StandardSoundManager final : public sound_manager {
		public:
			StandardSoundManager();
			~StandardSoundManager();
			bool initialize() override;
			void finalize() override;
			void update() override;

			std::shared_ptr<i_bgm_controller> get_bgm_controller(const std::shared_ptr<resource_base> &music_resource);
			std::shared_ptr<i_sound_effect_controller> get_sound_effect_controller(
				const std::shared_ptr<resource_base> &sound_resource);
			void reset() override;

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}
