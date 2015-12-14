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
		struct SceneDataForGameProcess;
		class SceneAccessorForGameProcess;
	}
	namespace game {
		class GameProcess;
		class GameProcessManager final: public core::SceneModule
			,public GameProcessManagerPublicInterface{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager();
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

			/*システムプロセス追加(システムプロセスはIDマップや名前マップに登録しない)
			削除不可。固有名をつける*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				SetupProcess_(new_proc, []() {return true; });
				auto pos = AddGameProcessToList_(priority, new_proc);
				return new_proc;
			}
		private:
			/*ゲームプロセス優先度リスト
			優先度の低い順に並べるためにmapを使用。要素の追加削除でイテレータっが無効にならないことからlistを使用*/
			using ProcessListType =std::list<std::shared_ptr<GameProcess>>;
			using ProcessPriorytyListType = std::map<int, ProcessListType>;
			ProcessPriorytyListType process_priority_list_;
			using PositionTypeAtPriorytyList = std::pair<ProcessPriorytyListType::iterator, ProcessListType::iterator>;
			/*IDマップ。<id,<優先度リストイテレータ、プロセスリストイテレータ>>*/
			using ProcessIDMapType = std::unordered_map<GameProcess*, PositionTypeAtPriorytyList>;
			ProcessIDMapType process_id_map_;
			/*名前IDマップ。<プロセス名,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, GameProcess*>;
			ProcessNameMapType process_name_id_map_;
			/*登録解除リスト*/
			std::vector<PositionTypeAtPriorytyList> dispose_list_;
			/*チェッカーを通過する最初のプロセスを取得*/
			utility::WeakPointer<GameProcess> GetProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const {
				for (const auto& sp : process_id_map_) {
					if (type_checker(*sp.second.second)) { return *sp.second.second; }
				}
				return nullptr;
			}
			/*名前からゲームプロセスを取得*/
			utility::WeakPointer<GameProcess> GetProcess(const std::string& name)override;
			/*ゲームプロセス作製*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority)override {
				auto np = creator(game_);
				SetupProcess_(np, [id = np.get(), this](){return RemoveGameProcess_(id); });
				auto pos = AddGameProcessToList_(priority, np);
				return RegisterToIDMap_(np.get(), pos) ? np : nullptr;
			}
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority,const std::string& name)override {
				auto np = creator(game_);
				SetupProcess_(np, [id = np.get(), this](){return RemoveGameProcess_(id); });
				auto pos = AddGameProcessToList_(priority, np);
				return (RegisterToIDMap_(np.get(), pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
			}

			/*プロセスの設定*/
			void SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover);
			/*プロセスをリストに追加*/
			PositionTypeAtPriorytyList AddGameProcessToList_(int priority, const std::shared_ptr<GameProcess>& game_process);
			/*名前を登録*/
			bool RegisterProcessName_(const std::string& name,GameProcess* id);
			/*IDマップにプロセス登録*/
			bool RegisterToIDMap_(GameProcess* id,const PositionTypeAtPriorytyList& pos_at_plist);
			/*IDでプロセス削除*/
			bool RemoveGameProcess_(GameProcess* id);
			/*破棄リストを処理する*/
			void ProcessDisposeList_();

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameProcess> scene_data_;
		};
	}
}