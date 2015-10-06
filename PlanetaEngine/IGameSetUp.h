#pragma once

#include <string>
#include "TwoKeyMap.h"

namespace planeta_engine {
	namespace core {
		class SceneManager;
		class IGameSetUp {
		public:
			virtual ~IGameSetUp() = 0 {};
			/*‰Šú‰»—pŠÖ”ŒQ*/
			virtual SceneManager& scene_manager() = 0;
			virtual void SetCollisionMap(const utility::TwoKeyHashMap<std::string, bool>& col_map) = 0;
			virtual void SetCollisionMap(utility::TwoKeyHashMap<std::string, bool>&& col_map) = 0;
		};
	}
}
