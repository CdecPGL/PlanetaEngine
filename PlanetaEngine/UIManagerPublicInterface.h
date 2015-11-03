#pragma once

#include <memory>
#include <functional>

namespace planeta_engine {
	namespace game {
		class UIObject;
		class UIManagerPublicInterface {
		public:
			virtual ~UIManagerPublicInterface() = default;
			template<class UIObjectType>
			std::shared_ptr<UIObjectType> CreateUIObject(int layer) {
				return CreateUIObject([] {return std::make_shared<UIObjectType>(); }, layer);
			}
			virtual bool RemoveLayer(int layer) = 0;
		private:
			virtual std::shared_ptr<UIObject> CreateUIObject(const std::function<std::shared_ptr<UIObject>()>& creator,int layer) = 0;
		};
	}
}
