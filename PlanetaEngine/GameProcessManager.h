#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "SceneModule.h"
#include "GameProcessManagerPublicInterface.h"
#include "WeakPointer.h"
#include "InsertPosIndication.h"
#include "GameProcessListTypes.h"

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
			削除不可*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(const core::GameProcessPositionInList& pos,InsertPosIndication ins_pos = InsertPosIndication::At) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				SetupProcess_(new_proc, []() {return true; });
				auto pos = AddGameProcessToList_(priority, new_proc);
				return new_proc;
			}
		private:
			/*ゲームプロセスリスト
			要素の追加削除でイテレータが無効にならないことからlistを使用*/
			core::GameProcessPriorytyListType process_priority_list_;
			/*IDマップ。<id,リスト内位置>*/
			using ProcessIDMapType = std::unordered_map<GameProcess*, core::GameProcessPositionTypeAtList>;
			ProcessIDMapType process_id_map_;
			/*名前IDマップ。<プロセス名,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, GameProcess*>;
			ProcessNameMapType process_name_id_map_;
			/*登録解除リスト*/
			std::vector<core::GameProcessPositionTypeAtList> dispose_list_;
			/*キーポジションマップ*/
			std::unordered_map<std::string, core::GameProcessPriorytyListType> key_position_map_;

			/*名前からゲームプロセスを取得*/
			utility::WeakPointer<GameProcess> GetProcess(const std::string& name)override;
			/*ゲームプロセス作製*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication)override;
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication, const std::string& name)override;
			/*キーポジションの設定*/
			bool SetKeyPosition(const core::GameProcessPositionInList& pos, const std::string& id)override;

			/*プロセスの設定*/
			void SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover,const core::GameProcessPositionInList& pos);
			/*プロセスをリストに追加*/
			core::GameProcessPositionTypeAtList AddGameProcessToList_(const std::shared_ptr<GameProcess>& game_process, const core::GameProcessPositionInList& pos, InsertPosIndication ins_pos);
			/*名前を登録*/
			bool RegisterProcessName_(const std::string& name,GameProcess* id);
			/*IDマップにプロセス登録*/
			bool RegisterToIDMap_(GameProcess* id,const core::GameProcessPositionTypeAtList& pos_at_plist);
			/*位置でプロセス削除*/
			bool RemoveGameProcess_(const core::GameProcessPositionInList& pos);
			/*破棄リストを処理する*/
			void ProcessDisposeList_();

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameProcess> scene_data_;
		};
	}
}