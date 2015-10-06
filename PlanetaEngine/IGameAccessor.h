#pragma once

#include <string>
#include "TwoKeyMap.h"

namespace planeta_engine {
	namespace core {
		class ISceneManagerAccessor;
		class KeyInputManager;
		class ResourceManager;
		class IGameAccessor {
		public:
			virtual ~IGameAccessor() = default;
			virtual core::ISceneManagerAccessor& scene_manager() = 0;
			virtual core::KeyInputManager& key_input_manager() = 0;
			virtual core::ResourceManager& resource_manager() = 0;
			virtual const utility::TwoKeyHashMap<std::string, bool>& GetCollisionMap()const = 0;
		};
	}
}