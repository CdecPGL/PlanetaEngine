#pragma once

#include <unordered_map>
#include <memory>

namespace planeta_engine {
	class UIComponent;
	namespace game {
		class IUIComponentContainer {
		public:
			void UpdateComponent();

			std::shared_ptr<UIComponent> AddComponent(const std::shared_ptr<UIComponent>& c);
			bool RemoveComponent(const std::shared_ptr<UIComponent>& c);
		private:
			std::unordered_map<void*, std::shared_ptr<UIComponent>> components_;
		};
	}
}