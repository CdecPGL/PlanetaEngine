#pragma once
#include "UIManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class UIManagerAccessorForSetUp final {
		public:
			explicit UIManagerAccessorForSetUp(UIManagerPublicInterface& umpi) :ui_managar_public_interface_(umpi) {}
			template<typename UIObjectType>
			std::shared_ptr<UIObjectType> CreateUIObject(int layer) { return ui_managar_public_interface_.CreateUIObject(layer); }
		private:
			UIManagerPublicInterface& ui_managar_public_interface_;
		};
	}
}
