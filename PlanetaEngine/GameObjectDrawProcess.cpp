#include "GameObjectDrawProcess.h"
#include "DrawComponent.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace system_processes {



		GameObjectDrawProcess::~GameObjectDrawProcess()
		{
		}

		bool GameObjectDrawProcess::Remove(const std::shared_ptr<components::DrawComponent>& draw_component)
		{
			auto it = _draw_component_map.find(draw_component.get());
			if (it == _draw_component_map.end()) { return false; }
			else {
				_draw_component_update_list[it->second.first].erase(it->second.second);
				_draw_component_map.erase(it);
				return true;
			}
		}

		void GameObjectDrawProcess::Update()
		{
			for (auto& dcl : _draw_component_update_list) {
				for (auto it = dcl.second.begin(); it != dcl.second.end(); ++it) {
					try {
						(*it)->Draw();
					}
					catch (utility::NullWeakPointerException& e) {
						debug::SystemLog::instance().LogError(std::string("DrawComponent::Drawで無効なWeakPointerが参照されました。問題の発生したコンポーネントはリストから除外されます。") + e.what(), "GameObjectDrawProcess::Update");
						dcl.second.erase(it);
						break;
					}
				}
			}
		}

	}
}
