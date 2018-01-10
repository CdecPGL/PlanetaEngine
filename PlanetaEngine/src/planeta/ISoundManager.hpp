#pragma once

#include <memory>
#include "planeta/IBGMController.hpp"
#include "ISoundEffectController.hpp"

namespace plnt {
	class ResourceBase;
	class ISoundManager {
	public:
		virtual ~ISoundManager() = 0 {};
		virtual std::shared_ptr<IBGMController> GetBGMController(const std::shared_ptr<ResourceBase>& music_resource) = 0;
		virtual std::shared_ptr<ISoundEffectController> GetSoundEffectController(const std::shared_ptr<ResourceBase>& sound_resource) = 0;
	};
}