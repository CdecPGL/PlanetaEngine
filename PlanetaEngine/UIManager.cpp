#include "UIManager.h"
//#include "ISceneAccessForUI.h"
//#include "IGameProcessManagerAccessor.h"

namespace planeta_engine {
	namespace game {
		bool UIManager::Process()
		{
			return true;
		}

		bool UIManager::Initialize()
		{
			return true;
		}

		void UIManager::Update()
		{
			for (auto& l : layers_) {
				l.second->Update();
			}
		}

		void UIManager::Draw()
		{
			for (auto& l : layers_) {
				l.second->Draw();
			}
		}

		bool UIManager::RemoveLayer(int layer)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { return false; }
			layers_.erase(it);
			return true;
		}

		void UIManager::Layer_::AddUIObject(const std::shared_ptr<UIObject>& o)
		{

		}

		void UIManager::Layer_::Update()
		{

		}

		void UIManager::Layer_::Draw()
		{

		}

	}
}
