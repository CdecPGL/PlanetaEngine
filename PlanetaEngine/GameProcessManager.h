#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "SceneModule.h"
#include "GameProcessManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class IGameAccessor;
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForGameProcess;
	}
	namespace game {
		class GameProcess;
		class GameProcessManager final: public core::SceneModule
			,public GameProcessManagerPublicInterface{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager() = default;
			/*初期化処理*/
			bool Initialize()override { return true; }
			/*終了処理*/
			void Finalize()override;
			/*シーンインターフェイスをセット*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*シーンデータをセット*/
			void SetSceneData(const core::SceneData& scene_data)override;

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
			/*システムプロセス追加
			削除不可。固有名をつける*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority, const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_, -1);
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
			/*チェッカーを通過するシステムプロセスを取得*/
			utility::WeakPointer<GameProcess> GetSystemProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const {
				for (const auto& sp : _system_process_map) {
					if (type_checker(*sp.second)) { return *sp.second; }
				}
				return nullptr;
			}
			/*プロセス追加(IDを返す。-1でエラー)*/
			int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process);
			/*ゲームプロセス作製*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority)override {
				auto np = creator(game_, _id_counter++);
				return AddGameProcess(priority, np) >= 0 ? np : nullptr;
			}
			/*システムプロセス追加*/
			bool CreateSystemProcess(const std::shared_ptr<GameProcess>& game_process, int priority) { return AddGameProcess(priority, game_process) >= 0; }
			/*プロセスの設定*/
			void SetupProcess(const std::shared_ptr<GameProcess>& game_process);

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
		};
	}
}