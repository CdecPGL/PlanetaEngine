#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "Object.h"
#include "IGameProcessManagerAccessor.h"
#include "IGameProcessManagerSetup.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class IGameAccessor;
		class ISceneAccessForGameProcess;
	}
	namespace game {
		class IUIManagerAccessor;
		class IGameObjectManagerAccessor;
		class GameProcess;
		class GameProcessManager final: public core::Object
			,public IGameProcessManagerAccessor,public IGameProcessManagerSetup{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager() = default;
			/*各種マネージャポインターのセット*/
			void SetManagerPointer(const utility::WeakPointer<core::ISceneAccessForGameProcess>& scene) { _scene = scene; }
			/*終了処理*/
			bool Finalize();
			/*プロセスのアップデート*/
			void Update();
			/*管理処理*/
			bool Process();
			/*IDでプロセス取得*/
			utility::WeakPointer<GameProcess> GetGameProcess(int id) override;
			/*IDでプロセス削除*/
			bool RemoveGameProcess(int id)override;


			/*名前でシステムプロセス取得*/
			utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name)override {
				auto it = _system_process_map.find(name);
				return it == _system_process_map.end() ? nullptr : *it->second;
			}
			/*型でシステムプロセス取得*/
			/*template<class C>
			std::shared_ptr<C> GetSystemProcess() {
				for (const auto& p : _system_process_map) {
					std::shared_ptr<C> ptr = std::dynamic_pointer_cast<C>(*p.second);
					if (ptr) { return ptr; }
				}
				return nullptr;
			}*/
			/*システムプロセス追加
			削除不可。固有名をつける*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority, const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(_game, -1);
				SetupProcess(new_proc);
				_game_processes[priority].emplace_back(new_proc);
				_system_process_map.emplace(name, --_game_processes[priority].end());
				return new_proc;
			}
		private:
			/*ゲームプロセスリスト
			優先度の低い順に並べるためにmapを使用。要素の追加削除でイテレータっが無効にならないことからlistを使用*/
			std::map<int, std::list<std::shared_ptr<GameProcess>>> _game_processes;
			/*IDマップ。<id,<優先度、プロセスリストのイテレータ>>*/
			std::unordered_map<int, std::pair<int, std::list<std::shared_ptr<GameProcess>>::iterator>> _id_map;

			/**/
			int _id_counter;
			/*登録解除IDリスト*/
			std::vector<int> _unresist_id_list;
			/*特殊プロセスマップ。<プロセス名,プロセスリストのイテレータ>*/
			std::unordered_map<std::string, std::list<std::shared_ptr<GameProcess>>::iterator> _system_process_map;
			std::unordered_map<std::string, std::list<std::shared_ptr<GameProcess>>::iterator>& _GetSystemProcessList()override { return _system_process_map; }
			/*プロセス追加(IDを返す。-1でエラー)*/
			int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process)override;
			/*あたしいIDを取得する*/
			int GetNewID()override { return _id_counter++; }
			core::IGameAccessor& GetGameAccessorReference()override { return _game; }
			/*プロセスの設定*/
			void SetupProcess(const std::shared_ptr<GameProcess>& game_process);

			core::IGameAccessor& _game;
			utility::WeakPointer<core::ISceneAccessForGameProcess> _scene;
		};
	}
}