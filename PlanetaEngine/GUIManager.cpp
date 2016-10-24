#include "GUIManager.h"
#include "GUIObject.h"
#include "SceneData.h"
#include "GUIManagerConnection.h"
#include "LogUtility.h"
#include "boost/lexical_cast.hpp"

namespace planeta {
	/*namespace game {
		bool GUIManager::Process()
		{
			return true;
		}

		bool GUIManager::Initialize()
		{
			return true;
		}

		void GUIManager::Update()
		{
			for (auto& l : layers_) {
				l.second.Update();
			}
		}

		void GUIManager::Draw(ScreenDrawerGUI& drawer)
		{
			for (auto& l : layers_) {
				l.second.Draw(drawer);
			}
			DebugDraw();
		}

		void GUIManager::DebugDraw()
		{
			for (auto& l : layers_) {
				l.second.DebugDraw();
			}
		}

		bool GUIManager::RemoveLayer(int layer)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { 
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false; 
			}
			layers_.erase(it);
			return true;
		}

		GUIManager::GUIManager()
		{

		}

		GUIManager::~GUIManager() = default;

		void GUIManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<SceneAccessorForGUI>(spi);
		}

		std::shared_ptr<GUIObject> GUIManager::CreateGUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator, int layer)
		{
			auto nuo = creator();
			if (nuo == nullptr) {
				debug::SystemLog::instance().LogError("UIObjectインスタンスの作成に失敗しました。", __FUNCTION__);
				return nullptr;
			}
			if (nuo->Initialize(std::make_unique<GUIManagerConnection>(*this, layer, nuo.get()))) {
				layers_[layer].AddGUIObject(nuo);
				return nuo;
			}
			else {
				debug::SystemLog::instance().LogError("UIObjectの初期化に失敗しました。", __FUNCTION__);
				return nullptr;
			}
		}

		bool GUIManager::RemoveGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { 
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.RemoveGUIObject(uo);
		}

		bool GUIManager::ShowGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.ShowGUIObject(uo);
		}

		bool GUIManager::CloseGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたレイヤーは存在しません。(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.CloseUIObject(uo);
		}

		void GUIManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForGUI>();
		}

		void GUIManager::Layer_::AddGUIObject(const std::shared_ptr<GUIObject>& o)
		{
			inactive_gui_objects_.emplace(o.get(), o);
		}

		void GUIManager::Layer_::Update()
		{
			for (auto& p : active_gui_objects_) {
				p.second->Update();
			}
		}

		void GUIManager::Layer_::Draw(ScreenDrawerGUI& drawer)
		{
			for (auto& p : active_gui_objects_) {
				p.second->Draw(drawer);
			}
		}

		void GUIManager::Layer_::DebugDraw()
		{
			for (auto& p : active_gui_objects_) {
				p.second->DebugDraw();
			}
		}

		bool GUIManager::Layer_::RemoveGUIObject(GUIObject* uo)
		{
			auto it = inactive_gui_objects_.find(uo);
			if (it != inactive_gui_objects_.end()) {
				inactive_gui_objects_.erase(it);
				return true;
			}
			it = active_gui_objects_.find(uo);
			if (it != active_gui_objects_.end()) {
				active_gui_objects_.erase(it);
				return true;
			}
			debug::SystemLog::instance().LogWarning(std::string("指定されたUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
			return false;
		}

		bool GUIManager::Layer_::ShowGUIObject(GUIObject* uo)
		{
			auto it = inactive_gui_objects_.find(uo);
			if (it == inactive_gui_objects_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたアクティブでないUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
				return false;
			}
			active_gui_objects_.emplace(it->first, it->second);
			inactive_gui_objects_.erase(it);
			return true;
		}

		bool GUIManager::Layer_::CloseUIObject(GUIObject* uo)
		{
			auto it = active_gui_objects_.find(uo);
			if (it == active_gui_objects_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("指定されたアクティブなUIObjectは存在しません。(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
				return false;
			}
			inactive_gui_objects_.emplace(it->first, it->second);
			active_gui_objects_.erase(it);
			return true;
		}

	}*/
}
