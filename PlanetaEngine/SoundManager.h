#pragma once

#include "ISoundManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		class SoundManager : public ISoundManager, private util::NonCopyable<SoundManager> {
		public:
			virtual ~SoundManager() = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
			virtual void Reset() = 0;
		};
	}
}