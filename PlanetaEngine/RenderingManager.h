#pragma once

#include "IRenderingManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		class Screen;
		/*レンダリングマネージャ*/
		class RenderingManager : public IRenderingManager, private util::NonCopyable<RenderingManager> {
		public:
			virtual ~RenderingManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual bool Update() = 0;
		};
	}
}