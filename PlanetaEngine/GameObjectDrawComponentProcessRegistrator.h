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
			/*描画コンポーネント登録*/
			void Register(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
			/*描画コンポーネント登録解除*/
			bool Remove(const std::shared_ptr<components::CDraw2D>& draw_component);
			/*描画優先度変更*/
			bool ChangePriority(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
		private:
			std::unordered_map<components::CDraw2D*, utility::WeakPointer<game::Task>> process_map_;
		};
	}
}
