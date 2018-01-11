#pragma once

#include "planeta/core/ILogManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	namespace private_ {
		/*ログマネージャ*/
		class LogManager : public ILogManager, public SubSystemManager {
		public:
			virtual ~LogManager()override = default;
			/*初期化*/
			virtual bool Initialize() = 0;
			/*終了処理*/
			virtual void Finalize() = 0;
		};
	}
}
