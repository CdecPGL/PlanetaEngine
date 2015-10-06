#pragma once

#include "Object.h"
#include "IUIManagerAccessor.h"
#include "IUIManagerSetup.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ISceneAccessForUI;
	}
	namespace ui_objects {
		class EmptyUIObject;
	}
	namespace game {
		class IGameProcessManagerAccessor;
		class UIManager final: public core::Object ,public IUIManagerAccessor,public IUIManagerSetup{
		public:

			void SetManagerPointer(const utility::WeakPointer<core::ISceneAccessForUI>& scene) { scene_ = scene; }
			/*I—¹ˆ—*/
			bool Finalize() { return true; }
			/*ŠÇ—ˆ—*/
			bool Process();
		private:
			utility::WeakPointer<core::ISceneAccessForUI> scene_;
			std::shared_ptr<ui_objects::EmptyUIObject> root_ui_object_;
		};
	}
}