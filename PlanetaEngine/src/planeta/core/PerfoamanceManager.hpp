#pragma once

#include "planeta/core/IPerformanceManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	namespace private_ {
		/*�p�t�H�[�}���X�}�l�[�W��*/
		class PerformanceManager : public IPerformanceManager, public SubSystemManager {
		public:
			virtual ~PerformanceManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void Update() = 0;
		};
	}
}
