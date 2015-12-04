#pragma once
#include "GameProcess.h"
#include <unordered_map>
#include <map>
#include <list>
#include <memory>

namespace planeta_engine {
	namespace components {
		class DrawComponent;
	}
	namespace system_processes {
		class GameObjectDrawProcess :
			public game::GameProcess
		{
		public:
			using GameProcess::GameProcess;
			~GameObjectDrawProcess();
			/*描画コンポーネント登録*/
			void Resist(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				_draw_component_update_list[priority].push_back(draw_component);
				_draw_component_map.emplace(draw_component.get(), std::make_pair(priority, --_draw_component_update_list[priority].end()));
			}
			/*描画コンポーネント登録解除*/
			bool Remove(const std::shared_ptr<components::DrawComponent>& draw_component);
			/*描画優先度変更*/
			bool ChangePriority(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				if (!Remove(draw_component)) { return false; }
				Resist(draw_component, priority);
				return true;
			}
		private:
			void Update()override final;
			/*更新リスト(mapとlistは要素の追加削除を行ってもイテレータが有効)*/
			std::map<int, std::list<std::shared_ptr<components::DrawComponent>>> _draw_component_update_list;
			/*描画コンポーネントマップ<ポインタ、更新リストへのイテレータ>*/
			std::unordered_map<components::DrawComponent*, std::pair<int, std::list<std::shared_ptr<components::DrawComponent>>::iterator>> _draw_component_map;
		};
	}
}
