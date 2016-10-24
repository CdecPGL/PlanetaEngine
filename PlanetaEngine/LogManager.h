#pragma once

#include "ILogManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		/*ログマネージャ*/
		class LogManager : public ILogManager, private util::NonCopyable<LogManager> {
		public:
			virtual ~LogManager()override = default;
			/*初期化*/
			virtual bool Initialize() = 0;
			/*終了処理*/
			virtual void Finalize() = 0;
		};
	}
}
