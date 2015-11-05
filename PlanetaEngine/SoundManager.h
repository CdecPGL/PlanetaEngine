#pragma once

#include "PointerSingletonTemplate.h"

namespace planeta_engine {
	namespace resources {
		class MusicResource;
		class SoundResource;
	}
	namespace core {
		class SoundManager final : public utility::PointerSingletonTemplate<SoundManager>{
		public:
			bool PlayBGM(const std::shared_ptr<resources::MusicResource>& music_resource);
			bool PlaySound(const std::shared_ptr<resources::SoundResource>& sound_resource);
			void StopAllBGM();
			void StopAllSound();
			void StopAllBGMAndSound();
		private:

		};
	}
}