#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include "NonCopyable.h"
#include "SceneModule.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class ScreenDrawerGUI;
	class CDraw2D;
	class CCamera2D;
	namespace core {
		class Screen;
		class GameObjectDrawSystem :public core::SceneModule, private util::NonCopyable<GameObjectDrawSystem>
		{
		public:
			GameObjectDrawSystem();
			bool Initialize()override;
			void Finalize()override;
			void Update()override;
			void ExcuteDraw();
			void ApplyCameraState();
			/*描画コンポーネント登録*/
			void Register(const std::shared_ptr<CDraw2D>& draw_component, int priority) {
				_draw_component_update_list[priority].push_back(draw_component);
				_draw_component_map.emplace(draw_component.get(), std::make_pair(priority, --_draw_component_update_list[priority].end()));
			}
			/*描画コンポーネント登録解除*/
			bool Remove(const std::shared_ptr<CDraw2D>& draw_component);
			/*描画優先度変更*/
			bool ChangePriority(const std::shared_ptr<CDraw2D>& draw_component, int priority) {
				if (!Remove(draw_component)) { return false; }
				Register(draw_component, priority);
				return true;
			}

			/*カメラコンポーネント登録*/
			bool RegisterCamera(const std::shared_ptr<CCamera2D>& camera_component);
			/*カメラコンポーネント登録解除*/
			void RemoveCamera(CCamera2D* camera_component);
		private:
			/*更新リスト(mapとlistは要素の追加削除を行ってもイテレータが有効)*/
			std::map<int, std::list<std::shared_ptr<CDraw2D>>> _draw_component_update_list;
			/*描画画面*/
			std::shared_ptr<Screen> screen_;
			/*描画コンポーネントマップ<ポインタ、更新リストへのイテレータ>*/
			std::unordered_map<CDraw2D*, std::pair<int, std::list<std::shared_ptr<CDraw2D>>::iterator>> _draw_component_map;
			std::unique_ptr<ScreenDrawer2D> screen_drawer_2d_;
			std::unique_ptr<ScreenDrawerGUI> screen_drawer_gui_;
			/*カメラコンポーネント*/
			std::shared_ptr<CCamera2D> camera2d_;
		};
	}
}
