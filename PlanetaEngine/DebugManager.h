#pragma once

#include "IDebugManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		class DebugManager : public IDebugManager, private util::NonCopyable<DebugManager> {
		public:
			virtual ~DebugManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
		};
	}
}