#pragma once

#include "planeta/ILogManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	namespace private_ {
		/*���O�}�l�[�W��*/
		class LogManager : public ILogManager, public SubSystemManager {
		public:
			virtual ~LogManager()override = default;
			/*������*/
			virtual bool Initialize() = 0;
			/*�I������*/
			virtual void Finalize() = 0;
		};
	}
}
