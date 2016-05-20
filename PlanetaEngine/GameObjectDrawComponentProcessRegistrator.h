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
			/*�`��R���|�[�l���g�o�^*/
			void Register(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
			/*�`��R���|�[�l���g�o�^����*/
			bool Remove(const std::shared_ptr<components::CDraw2D>& draw_component);
			/*�`��D��x�ύX*/
			bool ChangePriority(const std::shared_ptr<components::CDraw2D>& draw_component, int priority);
		private:
			std::unordered_map<components::CDraw2D*, utility::WeakPointer<game::Task>> process_map_;
		};
	}
}
