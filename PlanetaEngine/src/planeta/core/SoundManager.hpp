#pragma once

#include "ISoundManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	namespace private_ {
		class SoundManager : public ISoundManager, public SubSystemManager {
		public:
			virtual ~SoundManager() = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
			virtual void Reset() = 0;
		};
	}
}
