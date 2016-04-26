#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace components {
		class CDraw2D;
	}
	namespace game {
		class Task;
	}
	namespace core {
		class GameObjectDrawComponentProcessRegistrator {
		public:
			/*•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg“o˜^*/
			void Register(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
			/*•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg“o˜^‰ğœ*/
			bool Remove(const std::shared_ptr<components::CDraw2D>& draw_component);
			/*•`‰æ—Dæ“x•ÏX*/
			bool ChangePriority(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
		private:
			std::unordered_map<components::CDraw2D*, utility::WeakPointer<game::Task>> process_map_;
		};
	}
}
