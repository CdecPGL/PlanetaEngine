#pragma once

#include "ScenePublicInterface.h"
#include "GUIManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplGUIOperateFunctions {
		public:
			explicit SAImplGUIOperateFunctions(core::ScenePublicInterface& spi) :gmpi_(spi.gui_manager()) {}
			template<typename GUIObjectType>
			std::shared_ptr<GUIObjectType> CreateGUIObject(int layer) { return gmpi_.CreateGUIObject<GUIObjectType>(layer); }
			bool RemoveLayer(int layer) { gmpi_.RemoveLayer(layer); };
		private:
			game::GUIManagerPublicInterface& gmpi_;
		};
	}
}