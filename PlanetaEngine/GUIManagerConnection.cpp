#include "GUIManagerConnection.h"

#include "GUIManager.h"

namespace planeta_engine {
	namespace game {

		bool GUIManagerConnection::Show()
		{
			return gui_manager_.ShowGUIObject(layer_, pointer_);
		}

		bool GUIManagerConnection::Close()
		{
			return gui_manager_.CloseGUIObject(layer_, pointer_);
		}

		void GUIManagerConnection::Dispose()
		{
			gui_manager_.CloseGUIObject(layer_, pointer_);
		}

		core::SceneDataForGUI& GUIManagerConnection::RefSceneData() {
			return gui_manager_.RefSceneData();
		}

	}
}