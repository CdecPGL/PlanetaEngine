﻿#pragma once

#include "SoundManager.hpp"

namespace plnt {
	namespace private_ {
		class StandardSoundManager final : public SoundManager{
		public:
			StandardSoundManager();
			~StandardSoundManager();
			bool Initialize() override;
			void Finalize() override;
			void Update()override;

			std::shared_ptr<IBGMController> GetBGMController(const std::shared_ptr<ResourceBase>& music_resource);
			std::shared_ptr<ISoundEffectController> GetSoundEffectController(const std::shared_ptr<ResourceBase>& sound_resource);
			void Reset()override;
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}