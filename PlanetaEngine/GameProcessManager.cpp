#include "GameProcessManager.h"
#include "GameProcess.h"

namespace planeta_engine {
	namespace game {
		
		GameProcessManager::GameProcessManager(core::IGameAccessor& engine):_game(engine)
		{

		}

		void GameProcessManager::Update()
		{
			for (const auto& ps : _game_processes) {
				for (auto& p : ps.second) {
					p->Update();
				}
			}
		}

		bool GameProcessManager::Process()
		{
			//登録解除リストのプロセスを削除する
			for (int id : _unresist_id_list) {
				auto it = _id_map.find(id);
				if (it == _id_map.end()) { continue; }
				else {
					_game_processes[it->second.first].erase(it->second.second);
					_id_map.erase(it);
				}
			}
			_unresist_id_list.clear();
			return true;
		}

		utility::WeakPointer<GameProcess> GameProcessManager::GetGameProcess(int id)
		{
			auto it = _id_map.find(id);
			if (it == _id_map.end()) { return nullptr; }
			else { return *it->second.second; }
		}

		bool GameProcessManager::RemoveGameProcess(int id)
		{
			auto it = _id_map.find(id);
			if (it == _id_map.end()) { return false; }
			else {
				_unresist_id_list.push_back(id);
				return true;
			}
		}

		int GameProcessManager::AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process)
		{
			SetupProcess(game_process);
			_game_processes[priority].push_back(game_process); //リストに追加
			_id_map.insert(std::make_pair(game_process->id(), std::make_pair(priority, --_game_processes[priority].end())));
			return game_process->id();
		}

		bool GameProcessManager::Finalize()
		{
			_unresist_id_list.clear();
			_system_process_map.clear();
			_id_map.clear();
			_game_processes.clear();
			return true;
		}

		void GameProcessManager::SetupProcess(const std::shared_ptr<GameProcess>& game_process)
		{
			game_process->SetScene(_scene);
		}

	}
}