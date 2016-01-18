#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameProcessManager.h"
#include "SceneAccessorForGameObject.h"
#include "SystemLog.h"
#include "SceneDataForGameObject.h"
#include "SceneData.h"
#include "GameObjectSetUpper.h"

namespace planeta_engine{
	namespace game{
		GameObjectManager::~GameObjectManager() = default;

		bool GameObjectManager::Process()
		{
			RemoveProc_();
			return true;
		}

		int GameObjectManager::RegisterGameObject(const std::shared_ptr<GameObject>& go)
		{
			go->SetSceneAccessor(scene_accessor_);
			int id = _id_counter++;
			go->SetManagerConnection(std::make_unique<GameObjectManagerConnection>(*this, id));
			inactive_game_objects_.insert(std::make_pair(id, go));
			return id;
		}

		int GameObjectManager::RegisterGameObject(const std::shared_ptr<GameObject>& go, const std::string& name)
		{
			int id = RegisterGameObject(go);
			if (id < -1) { return -1; }
			name_id_map_.emplace(name, id);
			return id;
		}

		void GameObjectManager::TakeOver(const GameObjectManager& gom)
		{
			for (auto& go : gom.active_game_objects_){
				active_game_objects_.insert(std::move(go));
			}
		}

		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateGameObject(const std::string& game_object_create_id)
		{
			auto go = GameObject::Create();
			if (go == nullptr) {
				debug::SystemLog::instance().LogError("無効なゲームオブジェクトです。", __FUNCTION__);
				return nullptr;
			}
			if (RegisterGameObject(go) >= 0) {
				if (SetUpGameObject_(*go,game_object_create_id)) {
					return go;
				} else {
					debug::SystemLog::instance().LogError("ゲームオブジェクトの初期化に失敗しました。", __FUNCTION__);
					return nullptr;
				}
			}
			else { return nullptr; }
			
		}
		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateGameObject(const std::string& game_object_create_id,const std::string& name)
		{
			auto go = GameObject::Create();
			if (go == nullptr) {
				debug::SystemLog::instance().LogError("無効なゲームオブジェクトです。", __FUNCTION__);
				return nullptr;
			}
			if (RegisterGameObject(go, name) >= 0) {
				if (SetUpGameObject_(*go,game_object_create_id)) {
					return go;
				} else {
					debug::SystemLog::instance().LogError("ゲームオブジェクトの初期化に失敗しました。", __FUNCTION__);
					return nullptr;
				}
			}
			else { return nullptr; }
			
		}

		bool GameObjectManager::ActivateGameObject(int id)
		{
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->Activated_();
			active_game_objects_.insert(*it);
			inactive_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::InActivateGameObject(int id)
		{			
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
			it->second->InActivated_();
			inactive_game_objects_.insert(*it);
			active_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::RemoveGameObject(int id)
		{
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) {
				it = inactive_game_objects_.find(id);
				if (it==inactive_game_objects_.end()) { return false; }
				else {
					it->second->Finalize_();
					garbage_.push_back(it->second);
					inactive_game_objects_.erase(it);
					return true;
				}
			}
			else {
				it->second->InActivated_(); //アクティブリストにあったゲームオブジェクトは、無効化処理を行ってから破棄する
				it->second->Finalize_();
				garbage_.push_back(it->second);
				active_game_objects_.erase(it);
				return true;
			}
		}

		void GameObjectManager::RemoveAllGameObjects()
		{
			for (auto& go : active_game_objects_) {
				go.second->InActivated_();
				go.second->Finalize_();
			}
			active_game_objects_.clear();
			for (auto& go : inactive_game_objects_) {
				go.second->Finalize_();
			}
			inactive_game_objects_.clear();
		}

		bool GameObjectManager::Initialize()
		{
			return true;
		}

		void GameObjectManager::Finalize()
{
			RemoveAllGameObjects();
		}

		GameObjectManager::GameObjectManager() :_id_counter(0){};

		void GameObjectManager::SetSceneInterface(core::ScenePublicInterface& spi) {
			scene_accessor_ = std::make_shared<SceneAccessorForGameObject>(spi);
		}

		void GameObjectManager::Update() {
			for (auto it = active_game_objects_.begin(); it != active_game_objects_.end();) {
				try {
					(it++)->second->Update();
				} catch (utility::NullWeakPointerException& e) {
					debug::SystemLog::instance().LogError(std::string("GameObject::Updateで無効なWeakPointerが参照されました。問題の発生したGameObjectはリストから除外されます。") + e.what(), __FUNCTION__);
					active_game_objects_.erase(it);
					assert(false);
					break;
				}
			}
		}

		void GameObjectManager::RemoveProc_() {
			garbage_.clear();
		}

		void GameObjectManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForGameObject>();
			scene_data_->collision_detect_process = scene_data.collision_detect_process;
			scene_data_->draw_component_process_registrator = scene_data.draw_component_process_registrator;
		}

		bool GameObjectManager::SetUpGameObject_(GameObject& game_object, const std::string& setup_id)const {
			auto it = setupper_map_.find(setup_id);
			if (it == setupper_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ゲームオブジェクトのセットアップに失敗しました。GameObjectSetUpper\"", setup_id, "\"は存在しません。");
				return false;
			}
			if ((it->second)(game_object)) { return true; }
			else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ゲームオブジェクトのセットアップに失敗しました。GameObjectSetUpper\"", setup_id, "\"の実行に失敗しました。");
				return false;
			}
		}

		void GameObjectManager::RegisterGameObjectSetUpper(const std::string& game_object_create_id, std::shared_ptr<GameObjectSetUpper>&& game_object_setupper) {
			if (setupper_map_.emplace(game_object_create_id, [gosur = std::move(game_object_setupper)](GameObject& game_object){return (*gosur)(game_object); }).second == false) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "GameObjectSetUpper\"", game_object_create_id, "\"はすでに登録されています。");
			}
		}

	}
}