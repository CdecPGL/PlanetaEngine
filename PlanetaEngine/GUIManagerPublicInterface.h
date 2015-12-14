#pragma once

#include <memory>
#include <functional>

namespace planeta_engine {
	namespace game {
		class GUIObject;
		class GUIManagerPublicInterface {
		public:
			virtual ~GUIManagerPublicInterface() = default;
			template<class GUIObjectType>
			std::shared_ptr<GUIObjectType> CreateGUIObject(int layer) {
				return std::static_pointer_cast<GUIObjectType> (CreateGUIObject([] {return std::make_shared<GUIObjectType>(); }, layer));
			}
			virtual bool RemoveLayer(int layer) = 0;
		private:
			virtual std::shared_ptr<GUIObject> CreateGUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator,int layer) = 0;
		};
	}
}
