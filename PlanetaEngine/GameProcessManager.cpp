#include "GameProcessManager.h"
#include "SceneAccessorForGameProcess.h"
#include "GameProcess.h"
#include "SceneData.h"
#include "SceneDataForGameProcess.h"
#include "GameProcessRegistrationData.h"

namespace planeta_engine {
	namespace game {
		
		GameProcessManager::GameProcessManager(core::IGameAccessor& engine):game_(engine)
		{

		}

		GameProcessManager::~GameProcessManager() = default;

		void GameProcessManager::Update()
		{
			for (const auto& ps : process_priority_list_) {
				for (auto& p : ps.second) {
					p->Update();
				}
			}
		}

		bool GameProcessManager::Process()
		{
			//登録解除リストのプロセスを削除する
			ProcessDisposeList_();
			return true;
		}

		bool GameProcessManager::RemoveGameProcess_(GameProcess* id)
		{
			auto it = process_id_map_.find(id);
			if (it == process_id_map_.end()) { return false; }
			else {
				//登録解除リストに追加
				dispose_list_.push_back(it->second);
				return true;
			}
		}

		GameProcessManager::PositionTypeAtPriorytyList GameProcessManager::AddGameProcessToList_(int priority, const std::shared_ptr<GameProcess>& game_process)
		{
			auto pit = process_priority_list_.find(priority);
			if (pit == process_priority_list_.end()) {
				pit = process_priority_list_.emplace(priority, ProcessListType()).first;
			}
			pit->second.push_back(game_process); //リストに追加
			PositionTypeAtPriorytyList pos(pit, --pit->second.end());
			return std::move(pos);
		}

		void GameProcessManager::Finalize()
		{

			ProcessDisposeList_(); //破棄リスト処理
			//まだ存在するプロセスの終了処理を行う
			for (auto& pl : process_priority_list_) {
				for (auto& p : pl.second) {
					p->Dispose();
				}
			}
			//全部クリア
			dispose_list_.clear();
			process_name_id_map_.clear();
			process_id_map_.clear();
			process_priority_list_.clear();
		}

		void GameProcessManager::SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover)
		{
			core::GameProcessRegistrationData registration_data;
			registration_data.scene_accessor = scene_accessor_;
			registration_data.disposer = std::move(remover);
			game_process->SystemSetUp(registration_data, *scene_data_);
		}

		void GameProcessManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<core::SceneAccessorForGameProcess>(spi);
		}

		void GameProcessManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForGameProcess>();
			scene_data_->camera = scene_data.camera;
		}

		bool GameProcessManager::RegisterToIDMap_(GameProcess* id, const PositionTypeAtPriorytyList& pos_at_plist) {
			process_id_map_.emplace(id, pos_at_plist);
			return true;
		}

		void GameProcessManager::ProcessDisposeList_() {
			for (auto& pos : dispose_list_) {
				pos.first->second.erase(pos.second);
			}
			dispose_list_.clear();
		}

		bool GameProcessManager::RegisterProcessName_(const std::string& name, GameProcess* id) {
			process_name_id_map_.emplace(name, id);
			return true;
		}

		utility::WeakPointer<GameProcess> GameProcessManager::GetProcess(const std::string& name) {
			auto nit = process_name_id_map_.find(name);
			if (nit == process_name_id_map_.end()) { return nullptr; }
			auto pit = process_id_map_.find(nit->second);
			if (pit == process_id_map_.end()) {
				process_name_id_map_.erase(nit);
				return nullptr;
			} else {
				return *pit->second.second; 
			}
		}

	}
}