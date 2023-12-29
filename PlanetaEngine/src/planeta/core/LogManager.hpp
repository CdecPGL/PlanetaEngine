#pragma once

#include "i_log_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	namespace private_ {
		/*ログマネージャ*/
		class LogManager : public i_log_manager, public SubSystemManager {
		public:
			virtual ~LogManager() override = default;
			/*初期化*/
			virtual bool Initialize() = 0;
			/*終了処理*/
			virtual void Finalize() = 0;
		};
	}
}
