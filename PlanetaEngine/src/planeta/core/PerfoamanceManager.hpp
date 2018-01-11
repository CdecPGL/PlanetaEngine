#pragma once

#include "planeta/core/IPerformanceManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
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
