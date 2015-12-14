#pragma once

#include <memory>
#include <functional>

namespace planeta_engine {
	namespace game {
		class GUIObject;
		class GUIManagerPublicInterface {
		public:
			virtual ~GUIManagerPublicInterface() = default;
			template<class UIObjectType>
			std::shared_ptr<UIObjectType> CreateUIObject(int layer) {
				return std::static_pointer_cast<UIObjectType> (CreateUIObject([] {return std::make_shared<UIObjectType>(); }, layer));
			}
			virtual bool RemoveLayer(int layer) = 0;
		private:
			virtual std::shared_ptr<GUIObject> CreateUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator,int layer) = 0;
		};
	}
}
