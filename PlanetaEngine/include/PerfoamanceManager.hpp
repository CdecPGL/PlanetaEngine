#pragma once

#include "IPerformanceManager.hpp"
#include "SubSystemManager.hpp"

namespace planeta {
	namespace private_ {
		/*パフォーマンスマネージャ*/
		class PerformanceManager : public IPerformanceManager, public SubSystemManager {
		public:
			virtual ~PerformanceManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
		};
	}
}
