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
			/*�`��R���|�[�l���g�o�^*/
			void Register(const std::shared_ptr<components::DrawComponent>& draw_component, int priority);
			/*�`��R���|�[�l���g�o�^����*/
			bool Remove(const std::shared_ptr<components::DrawComponent>& draw_component);
			/*�`��D��x�ύX*/
			bool ChangePriority(const std::shared_ptr<components::DrawComponent>& draw_component, int priority);
		private:
			std::unordered_map<components::DrawComponent*, utility::WeakPointer<game::GameProcess>> process_map_;
		};
	}
}
