#pragma once

#include "IPerformanceManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		/*パフォーマンスマネージャ*/
		class PerformanceManager : public IPerformanceManager, private util::NonCopyable<PerformanceManager> {
		public:
			virtual ~PerformanceManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
		};
	}
}
