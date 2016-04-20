#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "SceneModule.h"
#include "TaskManagerPublicInterface.h"
#include "WeakPointer.h"
#include "InsertPosIndication.h"
#include "TaskListTypes.h"
#include "TaskPosition.h"

namespace planeta_engine{
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		class ScenePublicInterface;
		struct SceneData;
		struct SceneDataForTask;
	}
	namespace game {
		class Task;
		class TaskManager final: public core::SceneModule
			,public TaskManagerPublicInterface{
		public:
			TaskManager(core::IGameAccessor& game);
			~TaskManager();
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
			utility::WeakPointer<C> AddSystemProcess(const core::TaskPosition& pos) {
				static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				auto new_pos = AddTaskToList_(new_proc, pos);
				SetupProcess_(new_proc, []() {return true; }, core::TaskPosition(new_pos.iterator_at_priority_list));
				return new_proc;
			}
			/*キーポジションの設定。(渡されたIDの順番通りに作成される。既存のキーポジションは削除される)*/
			bool SetKeyPositions(const std::list<std::string>& key_position_ids);

			/*名前からゲームプロセスを取得*/
			utility::WeakPointer<Task> GetProcess(const std::string& name)override;
			/*ゲームプロセス作製*/
			std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos)override;
			std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos, const std::string& name)override;
			/*キーポジションを取得する*/
			core::TaskPosition GetKeyPosition(const std::string& id)const override;
		private:
			/*ゲームプロセスリスト
			要素の追加削除でイテレータが無効にならないことからlistを使用*/
			core::TaskPriorytyListType process_priority_list_;
			/*内部位置位置*/
			struct InternalPosition {
				InternalPosition(const core::TaskPriorytyListType::iterator& pl_it, const core::TaskListType::iterator& l_it) :iterator_at_priority_list(pl_it), iterator_at_list(l_it) {}
				core::TaskPriorytyListType::iterator iterator_at_priority_list;
				core::TaskListType::iterator iterator_at_list;
			};
			/*IDマップ。<id,リスト内位置>*/
			using ProcessIDMapType = std::unordered_map<Task*, InternalPosition>;
			ProcessIDMapType process_id_map_;
			/*名前IDマップ。<プロセス名,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, Task*>;
			ProcessNameMapType process_name_id_map_;
			/*登録解除リスト*/
			std::vector<InternalPosition> dispose_list_;
			/*キーポジションマップ<ID,<削除可能フラグ,位置>>*/
			std::unordered_map<std::string, core::TaskPosition> key_position_map_;

			/*プロセスの設定*/
			void SetupProcess_(const std::shared_ptr<Task>& game_process, std::function<bool()>&& remover,const core::TaskPosition& pos);
			/*プロセスをリストに追加*/
			InternalPosition AddTaskToList_(const std::shared_ptr<Task>& game_process, const core::TaskPosition& pos);
			/*名前を登録*/
			bool RegisterProcessName_(const std::string& name,Task* id);
			/*IDマップにプロセス登録*/
			bool RegisterToIDMap_(Task* id,const InternalPosition& pos_at_plist);
			/*位置でプロセス削除*/
			bool RemoveTask_(const InternalPosition& pos);
			/*破棄リストを処理する*/
			void ProcessDisposeList_();
			/*優先度リストでの追加位置を取得または作成する*/
			core::TaskPriorytyListType::iterator CreateOrGetInsertPositionAtPriorityList_(const core::TaskPosition& pos);

			core::IGameAccessor& game_;
			std::shared_ptr<SceneAccessorForTask> scene_accessor_;
			std::unique_ptr<core::SceneDataForTask> scene_data_;
		};
	}
}