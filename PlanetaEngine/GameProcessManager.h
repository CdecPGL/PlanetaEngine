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
#include "GameProcessPosition.h"

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

			/*システムプロセス追加(システムプロセスはIDマップや名前マップに登録されないため、削除不可能)*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(const core::GameProcessPosition& pos,InsertPosIndication ins_pos = InsertPosIndication::At) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				auto new_pos = AddGameProcessToList_(new_proc, pos, ins_pos);
				SetupProcess_(new_proc, []() {return true; }, core::GameProcessPosition(new_pos.iterator_at_priority_list));
				return new_proc;
			}
			/*キーポジションの設定。(渡されたIDの順番通りに作成される。既存のキーポジションは削除される)*/
			bool SetKeyPositions(const std::list<std::string>& key_position_ids);

			/*名前からゲームプロセスを取得*/
			utility::WeakPointer<GameProcess> GetProcess(const std::string& name)override;
			/*ゲームプロセス作製*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication)override;
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication, const std::string& name)override;
			/*キーポジションを取得する*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const override;
		private:
			/*ゲームプロセスリスト
			要素の追加削除でイテレータが無効にならないことからlistを使用*/
			core::GameProcessPriorytyListType process_priority_list_;
			/*内部位置位置*/
			struct InternalPosition {
				InternalPosition(const core::GameProcessPriorytyListType::iterator& pl_it, const core::GameProcessListType::iterator& l_it) :iterator_at_priority_list(pl_it), iterator_at_list(l_it) {}
				core::GameProcessPriorytyListType::iterator iterator_at_priority_list;
				core::GameProcessListType::iterator iterator_at_list;
			};
			/*IDマップ。<id,リスト内位置>*/
			using ProcessIDMapType = std::unordered_map<GameProcess*, InternalPosition>;
			ProcessIDMapType process_id_map_;
			/*名前IDマップ。<プロセス名,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, GameProcess*>;
			ProcessNameMapType process_name_id_map_;
			/*登録解除リスト*/
			std::vector<InternalPosition> dispose_list_;
			/*キーポジションマップ<ID,<削除可能フラグ,位置>>*/
			std::unordered_map<std::string, core::GameProcessPosition> key_position_map_;

			/*プロセスの設定*/
			void SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover,const core::GameProcessPosition& pos);
			/*プロセスをリストに追加*/
			InternalPosition AddGameProcessToList_(const std::shared_ptr<GameProcess>& game_process, const core::GameProcessPosition& pos, InsertPosIndication ins_pos);
			/*名前を登録*/
			bool RegisterProcessName_(const std::string& name,GameProcess* id);
			/*IDマップにプロセス登録*/
			bool RegisterToIDMap_(GameProcess* id,const InternalPosition& pos_at_plist);
			/*位置でプロセス削除*/
			bool RemoveGameProcess_(const InternalPosition& pos);
			/*破棄リストを処理する*/
			void ProcessDisposeList_();
			/*優先度リストでの追加位置を取得または作成する*/
			core::GameProcessPriorytyListType::iterator CreateOrGetInsertPositionAtPriorityList_(const core::GameProcessPosition& pos, InsertPosIndication ins_pos);

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameProcess> scene_data_;
		};
	}
}