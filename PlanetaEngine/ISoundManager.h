#pragma once

#include <memory>
#include "IBGMController.h"
#include "ISoundEffectController.h"

namespace planeta {
	namespace private_ {
		class ResourceBase;
	}
	class ISoundManager {
	public:
		virtual ~ISoundManager() = 0 {};
		virtual std::shared_ptr<IBGMController> GetBGMController(const std::shared_ptr<private_::ResourceBase>& music_resource) = 0;
		virtual std::shared_ptr<ISoundEffectController> GetSoundEffectController(const std::shared_ptr<private_::ResourceBase>& sound_resource) = 0;
	};
}