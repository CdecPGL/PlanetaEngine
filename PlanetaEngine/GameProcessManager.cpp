#include "GameProcessManager.h"
#include "SceneAccessorForGameProcess.h"
#include "GameProcess.h"
#include "SceneData.h"
#include "SceneDataForGameProcess.h"
#include "GameProcessRegistrationData.h"
#include "GameProcessPositionInList.h"
#include "SystemLog.h"

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
			auto priority_list_it = CreateOrGetInsertPositionAtPriorityList_(pos, ins_pos);
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
				if (pos.pos_in_priority_list->size() == 0) {
					process_priority_list_.erase(pos.pos_in_priority_list);
				}
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
			return RegisterToIDMap_(np.get(), new_pos) ? np : nullptr;
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication, const std::string& name) {
			auto np = creator(game_);
			auto new_pos = AddGameProcessToList_(np, pos, pos_indication);
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, new_pos);
			return (RegisterToIDMap_(np.get(), new_pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const std::string& key_pos_id, InsertPosIndication pos_indication) {
			auto it = key_position_map_.find(key_pos_id);
			if (it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "ゲームプロセスの作成に失敗しました。存在しないキーポジション\"", key_pos_id, "\"が指定されました。");
				return nullptr;
			}
			auto np = creator(game_);
			it->second->push_back(np);
			core::GameProcessPositionInList new_pos;
			new_pos.pos_in_priority_list = it->second;
			new_pos.pos_in_list = --it->second->end();
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, new_pos);
			return RegisterToIDMap_(np.get(), new_pos) ? np : nullptr;
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const std::string& key_pos_id, InsertPosIndication pos_indication, const std::string& name) {
			auto it = key_position_map_.find(key_pos_id);
			if (it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "ゲームプロセスの作成に失敗しました。存在しないキーポジション\"", key_pos_id, "\"が指定されました。");
				return nullptr;
			}
			auto np = creator(game_);
			it->second->push_back(np);
			core::GameProcessPositionInList new_pos;
			new_pos.pos_in_priority_list = it->second;
			new_pos.pos_in_list = --it->second->end();
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, new_pos);
			return (RegisterToIDMap_(np.get(), new_pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
		}

		bool GameProcessManager::CreateKeyPosition(const std::string& id, const core::GameProcessPositionInList& pos, InsertPosIndication ins_ind) {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it != key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, "すでに存在するキーポジション\"", id, "\"を再作成しようとしました。");
				return false;
			}
			auto priority_list_it = CreateOrGetInsertPositionAtPriorityList_(pos, ins_ind);
			key_position_map_.emplace(std::make_pair(id, priority_list_it));
			return true;
		}

		core::GameProcessPriorytyListType::iterator GameProcessManager::CreateOrGetInsertPositionAtPriorityList_(const core::GameProcessPositionInList& pos, InsertPosIndication ins_pos) {
			auto priority_list_it = pos.pos_in_priority_list;
			switch (ins_pos) {
			case planeta_engine::InsertPosIndication::At:
				break;
			case planeta_engine::InsertPosIndication::Before:
				priority_list_it = process_priority_list_.insert(priority_list_it, core::GameProcessListType()); //直前に挿入
				break;
			case planeta_engine::InsertPosIndication::After:
				++priority_list_it;
				if (priority_list_it == process_priority_list_.end()) { //次が最後だったら末尾に追加
					process_priority_list_.push_back(core::GameProcessListType());
					priority_list_it = --process_priority_list_.end();
				} else { //最後出なかったら次の要素の直前に挿入
					priority_list_it = process_priority_list_.insert(priority_list_it, core::GameProcessListType());
				}
				break;
			default:
				break;
			}
			return priority_list_it;
		}

		bool GameProcessManager::DeleteKeyPosition(const std::string& id) {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "キーポジション\"", id, "\"が存在しないため削除できませんでした。");
				return false;
			}
			//削除対象のリストが空だったら削除する
			if (key_pos_it->second->size() == 0) {
				key_position_map_.erase(key_pos_it);
			}
			return true;
		}

	}
}