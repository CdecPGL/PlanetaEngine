#pragma once

#include "TaskPosition.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	namespace game {
		class TaskManagerPublicInterface;
	}
	namespace scene_accessor_impl {
		class SAImplTaskKeyPositionFunctions {
		public:
			explicit SAImplTaskKeyPositionFunctions(core::ScenePublicInterface& spi);
			/*�L�[�|�W�V�������擾����*/
			core::TaskPosition GetKeyPosition(const std::string& id)const;
		private:
			game::TaskManagerPublicInterface& gpmpi_;
		};
	}
}