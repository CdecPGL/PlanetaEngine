#pragma once
#include "UIManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class UIManagerAccessorForGameProcess final{
		public:
			explicit UIManagerAccessorForGameProcess(UIManagerPublicInterface& umpi) :ui_managar_public_interface_(umpi) {}
			template<typename UIObjectType>
			std::shared_ptr<UIObjectType> CreateUIObject(int layer) { return ui_managar_public_interface_.CreateUIObject<UIObjectType>(layer); }
			bool RemoveLayer(int layer) { ui_managar_public_interface_.RemoveLayer(layer); };
		private:
			UIManagerPublicInterface& ui_managar_public_interface_;
		};
	}
}
