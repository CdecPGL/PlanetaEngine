#pragma once
#include <unordered_map>
#include <list>
#include <memory>
#include <array>
#include <type_traits>
#include <bitset>
#include "SceneModule.h"
#include "TaskManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		class ScenePublicInterface;
		struct SceneData;
		struct SceneDataForTask;
	}
	class Task;
	class TaskManager final : public core::SceneModule
		, public TaskManagerPublicInterface {
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
			SetupTask_(new_proc, []() {return true; }, core::TaskPosition(new_pos.iterator_at_priority_list));
			return new_proc;
		}

		/*名前からゲームプロセスを取得*/
		utility::WeakPointer<Task> GetProcess(const std::string& name)override;
		/*ゲームプロセス作製*/
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot)override;
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name)override;
	private:
		static constexpr int TASK_SLOT_0 = 0;
		static constexpr int COVCON_SLOT_0 = 1;
		static constexpr int TASK_SLOT_1 = 2;
		static constexpr int COVCON_SLOT_1 = 3;
		static constexpr int APPVEL_SLOT = 4;
		static constexpr int COLDET_SLOT = 5;
		static constexpr int TASK_SLOT_2 = 6;
		static constexpr int COVCON_SLOT_2 = 7;
		static constexpr int TASK_SLOT_3 = 8;
		static constexpr int COVCON_SLOT_3 = 9;
		static constexpr int TASK_SLOT_4 = 10;
		static constexpr int COVCON_SLOT_4 = 11;
		static constexpr int TASK_SLOT_5 = 12;
		static constexpr int COVCON_SLOT_5 = 13;
		static constexpr int UDTANM_SLOT = 14;
		static constexpr int TASK_SLOT_6 = 15;
		static constexpr int COVCON_SLOT_6 = 16;
		static constexpr int TASK_SLOT_7 = 17;
		static constexpr int COVCON_SLOT_7 = 18;
		static constexpr int DRAW_SLOT = 19;
		static constexpr int SLOT_COUNT = 20;
		//タスク群タイプ
		using TaskGroup = std::list<std::shared_ptr<Task>>;
		//スロットマップ
		std::array<int, TASK_SLOT_SIZE> slot_group_number_map_ = {
			TASK_SLOT_0,
			TASK_SLOT_1,
			TASK_SLOT_2,
			TASK_SLOT_3,
			TASK_SLOT_4,
			TASK_SLOT_5,
			TASK_SLOT_6,
			TASK_SLOT_7,
		};
		//タスク群リスト
		std::array<TaskGroup,SLOT_COUNT> task_group_list_;
		//有効スロットビットマップ
		std::bitset<SLOT_COUNT> valid_slot_bitmap_;

		/*内部位置*/
		struct InternalPosition {
			InternalPosition(int gn,const TaskGroup::iterator& it) :group_number(gn), iterator_at_task_group(it) {}
			int group_number;
			TaskGroup::iterator iterator_at_task_group;
		};
		/*IDマップ。<id,リスト内位置>*/
		using TaskIDMapType = std::unordered_map<Task*, InternalPosition>;
		TaskIDMapType task_id_map_;
		/*名前IDマップ。<タスク名,ID>*/
		using TaskNameMapType = std::unordered_map<std::string, Task*>;
		TaskNameMapType task_name_id_map_;
		/*登録解除リスト*/
		std::vector<InternalPosition> dispose_list_;

		/*プロセスの設定*/
		void SetupTask_(const std::shared_ptr<Task>& game_process, const InternalPosition& pos);
		/*タスクスロットからタスクグループ番号を取得*/
		int GetGroupNumberFromSlot(TaskSlot slot);
		/*名前を登録*/
		bool RegisterTaskName_(const std::string& name, Task* id);
		/*IDマップにプロセス登録*/
		bool RegisterToIDMap_(Task* id, const InternalPosition& pos_at_plist);
		/*位置でプロセス削除*/
		bool RemoveTask_(const InternalPosition& pos);
		/*破棄リストを処理する*/
		void TaskDisposeList_();

		core::IGameAccessor& game_;
		std::shared_ptr<SceneAccessorForTask> scene_accessor_;
		std::unique_ptr<core::SceneDataForTask> scene_data_;
	};
}