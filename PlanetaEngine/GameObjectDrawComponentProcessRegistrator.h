#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace components {
		class DrawComponent;
	}
	namespace game {
		class GameProcess;
	}
	namespace core {
		class GameObjectDrawComponentProcessRegistrator {
		public:
			/*•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg“o˜^*/
			void Register(const std::shared_ptr<components::DrawComponent>& draw_component, int priority);
			/*•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg“o˜^‰ğœ*/
			bool Remove(const std::shared_ptr<components::DrawComponent>& draw_component);
			/*•`‰æ—Dæ“x•ÏX*/
			bool ChangePriority(const std::shared_ptr<components::DrawComponent>& draw_component, int priority);
		private:
			std::unordered_map<components::DrawComponent*, utility::WeakPointer<game::GameProcess>> process_map_;
		};
	}
}
