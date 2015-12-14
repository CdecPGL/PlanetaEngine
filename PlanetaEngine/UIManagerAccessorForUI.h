#pragma once

#pragma once
#include "GUIManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class UIManagerAccessorForUI {
		public:
			explicit UIManagerAccessorForUI(GUIManagerPublicInterface& umpi) :ui_managar_public_interface_(umpi) {}
			template<typename UIObjectType>
			std::shared_ptr<UIObjectType> CreateUIObject(int layer) { return ui_managar_public_interface_.CreateUIObject<UIObjectType>(layer); }
			bool RemoveLayer(int layer) { ui_managar_public_interface_.RemoveLayer(layer); };
		private:
			GUIManagerPublicInterface& ui_managar_public_interface_;
		};
	}
}