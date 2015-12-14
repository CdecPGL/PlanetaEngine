#pragma once

#include "ScenePublicInterface.h"

#include "GUIManagerAccessorForGUI.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForGUI final {
		public:
			explicit SceneAccessorForGUI(ScenePublicInterface& spi) : gui_manager_accessor_(spi.gui_manager()) {}
			game::GUIManagerAccessorForGUI& gui_manager() { return gui_manager_accessor_; }
		private:
			game::GUIManagerAccessorForGUI gui_manager_accessor_;
		};
	}
}