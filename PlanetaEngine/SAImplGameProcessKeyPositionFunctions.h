#pragma once

#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	namespace game {
		class GameProcessManagerPublicInterface;
	}
	namespace scene_accessor_impl {
		class SAImplGameProcessKeyPositionFunctions {
		public:
			explicit SAImplGameProcessKeyPositionFunctions(core::ScenePublicInterface& spi);
			/*�L�[�|�W�V�������擾����*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const;
		private:
			game::GameProcessManagerPublicInterface& gpmpi_;
		};
	}
}