#pragma once

#include <memory>
#include "PointerSingletonTemplate.h"
#include "IBGMController.h"
#include "ISoundEffectController.h"

namespace planeta_engine {
	namespace core {
		class ResourceBase;
		class SoundManager final : public utility::PointerSingletonTemplate<SoundManager>{
			friend utility::PointerSingletonTemplate<SoundManager>;
		public:
			bool Initialize() override;
			bool Finalize() override;
			void Update();

			std::shared_ptr<IBGMController> GetBGMController(const std::shared_ptr<core::ResourceBase>& music_resource);
			std::shared_ptr<ISoundEffectController> GetSoundEffectController(const std::shared_ptr<core::ResourceBase>& sound_resource);
			void Reset();
		private:
			SoundManager();
			~SoundManager();
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}