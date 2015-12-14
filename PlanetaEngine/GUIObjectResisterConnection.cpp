#include "GUIObjectResisterConnection.h"

#include "GUIManager.h"

namespace planeta_engine {
	namespace game {

		bool GUIObjectResisterConnection::Show()
		{
			return ui_manager_.ShowGUIObject(layer_, pointer_);
		}

		bool GUIObjectResisterConnection::Close()
		{
			return ui_manager_.CloseGUIObject(layer_, pointer_);
		}

		void GUIObjectResisterConnection::Dispose()
		{
			ui_manager_.CloseGUIObject(layer_, pointer_);
		}

	}
}