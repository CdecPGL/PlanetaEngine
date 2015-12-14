#include "GUIManager.h"
#include "GUIObject.h"
#include "SceneAccessorForGUI.h"
#include "GUIObjectResisterConnection.h"
#include "SystemLog.h"
#include "boost/lexical_cast.hpp"

namespace planeta_engine {
	namespace game {
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

		void GUIManager::Draw()
		{
			for (auto& l : layers_) {
				l.second.Draw();
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
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ���C���[�͑��݂��܂���B(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false; 
			}
			layers_.erase(it);
			return true;
		}

		GUIManager::GUIManager()
		{

		}

		void GUIManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<core::SceneAccessorForGUI>(spi);
		}

		std::shared_ptr<GUIObject> GUIManager::CreateGUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator, int layer)
		{
			auto nuo = creator();
			if (nuo == nullptr) {
				debug::SystemLog::instance().LogError("UIObject�C���X�^���X�̍쐬�Ɏ��s���܂����B", __FUNCTION__);
				return nullptr;
			}
			if (nuo->Initialize(std::make_unique<GUIObjectResisterConnection>(*this, layer, nuo.get()))) {
				layers_[layer].AddGUIObject(nuo);
				return nuo;
			}
			else {
				debug::SystemLog::instance().LogError("UIObject�̏������Ɏ��s���܂����B", __FUNCTION__);
				return nullptr;
			}
		}

		bool GUIManager::RemoveGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) { 
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ���C���[�͑��݂��܂���B(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.RemoveGUIObject(uo);
		}

		bool GUIManager::ShowGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ���C���[�͑��݂��܂���B(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.ShowGUIObject(uo);
		}

		bool GUIManager::CloseGUIObject(int layer, GUIObject* uo)
		{
			auto it = layers_.find(layer);
			if (it == layers_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ���C���[�͑��݂��܂���B(Layer ") + boost::lexical_cast<std::string>(layer) + ")", __FUNCTION__);
				return false;
			}
			return it->second.CloseUIObject(uo);
		}

		void GUIManager::SetSceneData(const core::SceneData& scene_data) {

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

		void GUIManager::Layer_::Draw()
		{
			for (auto& p : active_gui_objects_) {
				p.second->Draw();
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
			debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽUIObject�͑��݂��܂���B(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
			return false;
		}

		bool GUIManager::Layer_::ShowGUIObject(GUIObject* uo)
		{
			auto it = inactive_gui_objects_.find(uo);
			if (it == inactive_gui_objects_.end()) {
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ�A�N�e�B�u�łȂ�UIObject�͑��݂��܂���B(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
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
				debug::SystemLog::instance().LogWarning(std::string("�w�肳�ꂽ�A�N�e�B�u��UIObject�͑��݂��܂���B(Type ") + uo->GetType().name() + ", Pointer " + boost::lexical_cast<std::string>(uo) + ")", __FUNCTION__);
				return false;
			}
			inactive_gui_objects_.emplace(it->first, it->second);
			active_gui_objects_.erase(it);
			return true;
		}

	}
}