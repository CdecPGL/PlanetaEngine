#pragma once
#include "GUIManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GUIManagerAccessorForGameProcess final{
		public:
			explicit GUIManagerAccessorForGameProcess(GUIManagerPublicInterface& umpi) :gui_managar_public_interface_(umpi) {}
			template<typename GUIObjectType>
			std::shared_ptr<GUIObjectType> CreateGUIObject(int layer) { return gui_managar_public_interface_.CreateGUIObject<GUIObjectType>(layer); }
			bool RemoveLayer(int layer) { gui_managar_public_interface_.RemoveLayer(layer); };
		private:
			GUIManagerPublicInterface& gui_managar_public_interface_;
		};
	}
}
