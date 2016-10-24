#pragma once

#include "ILogManager.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		/*���O�}�l�[�W��*/
		class LogManager : public ILogManager, private util::NonCopyable<LogManager> {
		public:
			virtual ~LogManager()override = default;
			/*������*/
			virtual bool Initialize() = 0;
			/*�I������*/
			virtual void Finalize() = 0;
		};
	}
}
