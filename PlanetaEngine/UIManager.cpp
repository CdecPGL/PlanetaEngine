#include "UIManager.h"
#include "UIObject.h"
#include "SceneAccessorForUI.h"
#include "UIObjectResisterConnection.h"
#include "SystemLog.h"
#include "boost/lexical_cast.hpp"

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
				l.second.Update();
			}
		}

		void UIManager::Draw()
		{
			for (auto& l : layers_) {
				l.second.Draw();
			}
			DebugDraw();
		}

		void UIManager::DebugDraw()
		{
			for (auto& l : layers_) {
				l.second.DebugDraw();
			}
		}

		bool UIManager::RemoveLayer(int layer)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { 
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false; 
			}
			layers_.erase(it);
			return true;
		}

		UIManager::UIManager()
		{

		}

		void UIManager::SetScene(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<core::SceneAccessorForUI>(spi);
		}

		std::shared_ptr<UIObject> UIManager::CreateUIObject(const std::function<std::shared_ptr<UIObject>()>& creator, int layer)
		{
			auto nuo = creator();
			if (nuo == nullptr) {
				debug::SystemLog::instance().LogError("UIObjectインスタンスの作成に失敗しました。", __FUNCTION__);
				return nullptr;
			}
			if (nuo->Initialize(std::make_unique<UIObjectResisterConnection>(*this, layer, nuo.get()))) {
				layers_[layer].AddUIObject(nuo);
				return nuo;
			}
			else {
				debug::SystemLog::instance().LogError("UIObjectの初期化に失敗しました。", __FUNCTION__);
				return nullptr;
			}
		}

		bool UIManager::RemoveUIObject(int layer, UIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { 
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.RemoveUIObject(uo);
		}

		bool UIManager::ShowUIObject(int layer, UIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.ShowUIObject(uo);
		}

		bool UIManager::CloseUIObject(int layer, UIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.CloseUIObject(uo);
		}

		void UIManager::Layer_::AddUIObject(const std::shared_ptr<UIObject>& o)
		{
			inactive_ui_objects_.emplace(o.get(), o);
		}

		void UIManager::Layer_::Update()
		{
			for (auto& p : active_ui_objects_) {
				p.second->Update();
			}
		}

		void UIManager::Layer_::Draw()
		{
			for (auto& p : active_ui_objects_) {
				p.second->Draw();
			}
		}

		void UIManager::Layer_::DebugDraw()
		{
			for (auto& p : active_ui_objects_) {
				p.second->DebugDraw();
			}
		}

		bool UIManager::Layer_::RemoveUIObject(UIObject* uo)
		{
			auto it = inactive_ui_objects_.find(uo);
			if (it != inactive_ui_objects_.end()) {
				inactive_ui_objects_.erase(it);
				return true;
			}
			it = active_ui_objects_.find(uo);
			if (it != active_ui_objects_.end()) {
				active_ui_objects_.erase(it);
				return true;
			}
			debug::SystemLog::instance().LogWarning(std::string("指定されたUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
			return false;
		}

		bool UIManager::Layer_::ShowUIObject(UIObject* uo)
		{
			auto it = inactive_ui_objects_.find(uo);
			if (it == inactive_ui_objects_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたアクティブでないUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
				return false;
			}
			active_ui_objects_.emplace(it->first, it->second);
			inactive_ui_objects_.erase(it);
			return true;
		}

		bool UIManager::Layer_::CloseUIObject(UIObject* uo)
		{
			auto it = active_ui_objects_.find(uo);
			if (it == active_ui_objects_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたアクティブなUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
				return false;
			}
			inactive_ui_objects_.emplace(it->first, it->second);
			active_ui_objects_.erase(it);
			return true;
		}

	}
}
