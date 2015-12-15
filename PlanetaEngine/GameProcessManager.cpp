#include "GameProcessManager.h"
#include "SceneAccessorForGameProcess.h"
#include "GameProcess.h"
#include "SceneData.h"
#include "SceneDataForGameProcess.h"
#include "GameProcessRegistrationData.h"
#include "GameProcessPositionInList.h"

namespace planeta_engine {
	namespace game {
		
		GameProcessManager::GameProcessManager(core::IGameAccessor& engine):game_(engine)
		{

		}

		GameProcessManager::~GameProcessManager() = default;

		void GameProcessManager::Update()
		{
			for (const auto& ps : process_priority_list_) {
				for (auto& p : ps) {
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

		bool GameProcessManager::RemoveGameProcess_(const core::GameProcessPositionInList& pos)
		{
			//登録解除リストに追加
			dispose_list_.push_back(pos);
			return true;
		}

		core::GameProcessPositionTypeAtList GameProcessManager::AddGameProcessToList_(const std::shared_ptr<GameProcess>& game_process, const core::GameProcessPositionInList& pos, InsertPosIndication ins_pos)
		{
			auto priority_list_it = pos.pos_in_priority_list;
			switch (ins_pos) {
			case planeta_engine::InsertPosIndication::At:
				break;
			case planeta_engine::InsertPosIndication::Before:
				--priority_list_it;
				break;
			case planeta_engine::InsertPosIndication::After:
				++priority_list_it;
				break;
			default:
				break;
			}
			priority_list_it->push_back(game_process);
			auto list_it = --priority_list_it->end();

			core::GameProcessPositionInList insed_pos;
			insed_pos.pos_in_priority_list = priority_list_it;
			insed_pos.pos_in_list = list_it;
			return std::move(insed_pos);
		}

		void GameProcessManager::Finalize()
		{

			ProcessDisposeList_(); //破棄リスト処理
			//まだ存在するプロセスの終了処理を行う
			for (auto& pl : process_priority_list_) {
				for (auto& p : pl) {
					p->Dispose();
				}
			}
			//全部クリア
			dispose_list_.clear();
			process_name_id_map_.clear();
			process_id_map_.clear();
			process_priority_list_.clear();
		}

		void GameProcessManager::SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover, const core::GameProcessPositionInList& pos)
		{
			core::GameProcessRegistrationData registration_data;
			registration_data.scene_accessor = scene_accessor_;
			registration_data.disposer = std::move(remover);
			registration_data.position_in_list = pos;
			game_process->SystemSetUpAndInitialize(registration_data, *scene_data_);
		}

		void GameProcessManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<core::SceneAccessorForGameProcess>(spi);
		}

		void GameProcessManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForGameProcess>();
		}

		bool GameProcessManager::RegisterToIDMap_(GameProcess* id, const core::GameProcessPositionTypeAtList& pos_at_plist) {
			process_id_map_.emplace(id, pos_at_plist);
			return true;
		}

		void GameProcessManager::ProcessDisposeList_() {
			for (auto& pos : dispose_list_) {
				pos.pos_in_priority_list->erase(pos.pos_in_list);
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
				return *pit->second.pos_in_list;
			}
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication) {
			auto np = creator(game_);
			auto new_pos = AddGameProcessToList_(np, pos, pos_indication);
			SetupProcess_(np, [new_pos, this](){return RemoveGameProcess_(new_pos); }, new_pos);
			return RegisterToIDMap_(np.get(), pos) ? np : nullptr;
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication, const std::string& name) {
			auto np = creator(game_);
			auto new_pos = AddGameProcessToList_(np, pos, pos_indication);
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, new_pos);
			return (RegisterToIDMap_(np.get(), pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
		}

	}
}