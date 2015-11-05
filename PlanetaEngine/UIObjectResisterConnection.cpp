#include "UIObjectResisterConnection.h"

#include "UIManager.h"

namespace planeta_engine {
	namespace game {

		bool UIObjectResisterConnection::Show()
		{
			return ui_manager_.ShowUIObject(layer_, pointer_);
		}

		bool UIObjectResisterConnection::Close()
		{
			return ui_manager_.CloseUIObject(layer_, pointer_);
		}

		void UIObjectResisterConnection::Dispose()
		{
			ui_manager_.CloseUIObject(layer_, pointer_);
		}

	}
}