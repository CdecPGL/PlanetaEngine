#pragma once

#include "ScenePublicInterface.h"

#include "UIManagerAccessorForUI.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForUI final {
		public:
			explicit SceneAccessorForUI(ScenePublicInterface& spi) : ui_manager_accessor_(spi.ui_manager()) {}
			game::UIManagerAccessorForUI& ui_manager() { return ui_manager_accessor_; }
		private:
			game::UIManagerAccessorForUI ui_manager_accessor_;
		};
	}
}