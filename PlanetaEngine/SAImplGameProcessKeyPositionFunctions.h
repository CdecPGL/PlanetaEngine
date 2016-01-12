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
			/*キーポジションを取得する*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const;
		private:
			game::GameProcessManagerPublicInterface& gpmpi_;
		};
	}
}