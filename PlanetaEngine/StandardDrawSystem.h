#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include "DrawSystem.h"

namespace planeta {
	class ScreenDrawer2D;
	class ScreenDrawerGUI;
	namespace private_ {
		class Screen;
		class StandardDrawSystem final:public DrawSystem
		{
		public:
			StandardDrawSystem();
			~StandardDrawSystem();
			bool Initialize()override;
			void Finalize()override;
			void Update()override;
			void ExcuteDraw()override;
			void ApplyCameraState()override;
			/*描画コンポーネント登録*/
			void Register(const std::shared_ptr<CDraw2D>& draw_component, int priority)override {
				_draw_component_update_list[priority].push_back(draw_component);
				_draw_component_map.emplace(draw_component.get(), std::make_pair(priority, --_draw_component_update_list[priority].end()));
			}
			/*描画コンポーネント登録解除*/
			bool Remove(const std::shared_ptr<CDraw2D>& draw_component)override;
			/*描画優先度変更*/
			bool ChangePriority(const std::shared_ptr<CDraw2D>& draw_component, int priority)override {
				if (!Remove(draw_component)) { return false; }
				Register(draw_component, priority);
				return true;
			}

			/*カメラコンポーネント登録*/
			bool RegisterCamera(const std::shared_ptr<CCamera2D>& camera_component)override;
			/*カメラコンポーネント登録解除*/
			void RemoveCamera(CCamera2D* camera_component)override;

			void DebugInformationAddHandle(IDebugInformationAdder& di_adder) override;

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
