#include "GameObjectDrawSystem.h"
#include "CDraw2D.h"
#include "SystemLog.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"
#include "DrawManager.h"

namespace planeta_engine{
	namespace core {
		GameObjectDrawSystem::GameObjectDrawSystem()
		{

		}

		bool GameObjectDrawSystem::Remove(const std::shared_ptr<CDraw2D>& draw_component)
		{
			auto it = _draw_component_map.find(draw_component.get());
			if (it == _draw_component_map.end()) { return false; }
			else {
				_draw_component_update_list[it->second.first].erase(it->second.second);
				_draw_component_map.erase(it);
				return true;
			}
		}

		void GameObjectDrawSystem::ExcuteDraw()
		{
			//2DDraw
			for (auto& dcl : _draw_component_update_list) {
				for (auto it = dcl.second.begin(); it != dcl.second.end(); ++it) {
					try {
						(*it)->Draw(*screen_drawer_2d_);
					}
					catch (utility::NullWeakPointerException& e) {
						debug::SystemLog::instance().LogError(std::string("DrawComponent::Drawで無効なWeakPointerが参照されました。問題の発生したコンポーネントはリストから除外されます。") + e.what(), "GameObjectDrawProcess::Update");
						dcl.second.erase(it);
						break;
					}
				}
			}
		}

		void GameObjectDrawSystem::Update() {

		}

		bool GameObjectDrawSystem::Initialize() {
			screen_ = DrawManager::instance().CreateScreen();
			if (!screen_) { return false; }
			screen_drawer_2d_ = std::make_unique<ScreenDrawer2D>(*screen_);
			screen_drawer_gui_ = std::make_unique<ScreenDrawerGUI>(*screen_);
			return true;
		}

		void GameObjectDrawSystem::Finalize() {
			DrawManager::instance().DisposeScreen(screen_);
			return;
		}

	}
}
