#pragma once

#include "SoundManager.hpp"

namespace plnt {
	namespace private_ {
		class StandardSoundManager final : public SoundManager {
		public:
			StandardSoundManager();
			~StandardSoundManager();
			bool Initialize() override;
			void Finalize() override;
			void Update() override;

			std::shared_ptr<i_bgm_controller> get_bgm_controller(const std::shared_ptr<ResourceBase> &music_resource);
			std::shared_ptr<i_sound_effect_controller> get_sound_effect_controller(
				const std::shared_ptr<ResourceBase> &sound_resource);
			void Reset() override;

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}
