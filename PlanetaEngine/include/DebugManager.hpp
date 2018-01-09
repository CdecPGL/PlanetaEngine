#pragma once

#include "IDebugManager.hpp"
#include "SubSystemManager.hpp"

namespace planeta {
	namespace private_ {
		class RenderingManager;
		class DebugManager : public IDebugManager, public SubSystemManager {
		public:
			virtual ~DebugManager()override = default;
			virtual bool Initialize(RenderingManager& rendering_mgr) = 0;
			virtual void Finalize() = 0;
			virtual void PreRenderingUpdate() = 0;
			virtual void PostRenderingUpdate() = 0;
		};
	}
}