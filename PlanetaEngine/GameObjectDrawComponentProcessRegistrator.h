#pragma once

#include <memory>

namespace planeta_engine {
	namespace components {
		class DrawComponent;
	}
	namespace core {
		class GameObjectDrawComponentProcessRegistrator {
		public:
			/*描画コンポーネント登録*/
			void Register(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				
			}
			/*描画コンポーネント登録解除*/
			bool Remove(const std::shared_ptr<components::DrawComponent>& draw_component);
			/*描画優先度変更*/
			bool ChangePriority(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				
				return true;
			}
		private:

		};
	}
}
