#pragma once
#include "GUIManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GUIManagerAccessorForSetUp final {
		public:
			explicit GUIManagerAccessorForSetUp(GUIManagerPublicInterface& umpi) :gui_managar_public_interface_(umpi) {}
			template<typename GUIObjectType>
			std::shared_ptr<GUIObjectType> CreateGUIObject(int layer) { return gui_managar_public_interface_.CreateGUIObject<GUIObjectType>(layer); }
		private:
			GUIManagerPublicInterface& gui_managar_public_interface_;
		};
	}
}