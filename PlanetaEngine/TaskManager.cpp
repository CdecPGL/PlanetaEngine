#include <unordered_map>
#include <list>
#include <array>
#include <type_traits>
#include <bitset>
#include "TaskManager.h"
#include "Task.h"
#include "SceneData.h"
#include "TaskManagerConnection.h"
#include "SystemLog.h"
#include "SystemTaskSlot.h"

namespace planeta_engine {
	namespace {
		constexpr int TASK_SLOT_0 = 0;
		constexpr int COVCON_SLOT_0 = 1;
		constexpr int TASK_SLOT_1 = 2;
		constexpr int COVCON_SLOT_1 = 3;
		constexpr int APPVEL_SLOT = 4;
		constexpr int COLDET_SLOT = 5;
		constexpr int TASK_SLOT_2 = 6;
		constexpr int COVCON_SLOT_2 = 7;
		constexpr int TASK_SLOT_3 = 8;
		constexpr int COVCON_SLOT_3 = 9;
		constexpr int TASK_SLOT_4 = 10;
		constexpr int COVCON_SLOT_4 = 11;
		constexpr int TASK_SLOT_5 = 12;
		constexpr int COVCON_SLOT_5 = 13;
		constexpr int TASK_SLOT_6 = 14;
		constexpr int COVCON_SLOT_6 = 15;
		constexpr int UDTANM_SLOT = 16;
		constexpr int TASK_SLOT_7 = 17;
		constexpr int COVCON_SLOT_7 = 18;
		constexpr int TASK_SLOT_8 = 19;
		constexpr int COVCON_SLOT_8 = 20;
		constexpr int DRAW_SLOT = 21;
		constexpr int SLOT_COUNT = 22;
		//スロットマップ
		constexpr std::array<int, TASK_SLOT_SIZE> slot_group_number_map_ = {
			TASK_SLOT_0,
			TASK_SLOT_1,
			TASK_SLOT_2,
			TASK_SLOT_3,
			TASK_SLOT_4,
			TASK_SLOT_5,
			TASK_SLOT_6,
			TASK_SLOT_7,
			TASK_SLOT_8,
		};
		//システムスロットマップ
		using namespace core;
		constexpr std::array<int, SYSTEM_TASK_SLOT_SIZE> system_slot_group_number_map_ = {
			COVCON_SLOT_0,
			COVCON_SLOT_1,
			APPVEL_SLOT,
			COLDET_SLOT,
			COVCON_SLOT_2,
			COVCON_SLOT_3,
			COVCON_SLOT_4,
			COVCON_SLOT_5,
			COVCON_SLOT_6,
			UDTANM_SLOT,
			COVCON_SLOT_7,
			COVCON_SLOT_8,
			DRAW_SLOT,
		};
		/*タスクスロットからタスクグループ番号を取得*/
		int GetGroupNumberFromSlot(TaskSlot slot) {
			return slot_group_number_map_[static_cast<std::underlying_type_t<TaskSlot>>(slot)];
		}
		/*システムタスクスロットからタスクグループ番号を取得*/
		int GetGroupNumberFromSystemSlot(core::SystemTaskSlot slot) {
			return system_slot_group_number_map_[static_cast<std::underlying_type_t<core::SystemTaskSlot>>(slot)];
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class TaskManager::Impl_ {
	public:
		struct TaskData;
		core::IGameAccessor& game_;
		//タスク群タイプ
		using TaskGroupType = std::list<Task*>;
		//タスクリストタイプ
		using TaskListType = std::list<std::shared_ptr<TaskData>>;
		//名前マップタイプ
		using NameMapType = std::unordered_map<std::string, std::shared_ptr<TaskData>>;
		/*タスクの状態(順に、実行中、停止中、破棄された)*/
		enum class TaskState { Running, Pausing, Disposed };
		/*タスクの情報*/
		struct TaskData {
			TaskData() = default;
			TaskData(const TaskData&) = delete;
			TaskData& operator=(const TaskData&) = delete;
			std::shared_ptr<Task> task;
			TaskListType::iterator iterator_at_task_list;
			NameMapType::iterator iterator_at_name_map;
			bool is_named = false;
			int group_number; //所属しているタスクグループ番号
			TaskGroupType::iterator iterator_at_runnning_task_group; //所属しているタスクグループでのイテレータ
			TaskState state = TaskState::Pausing;
		};
	private:
		//タスク群リスト
		std::array<TaskGroupType, SLOT_COUNT> running_task_group_list_;
		//有効スロットビットマップ
		std::bitset<SLOT_COUNT> valid_slot_bitmap_;

		/*タスクリスト*/
		TaskListType task_list_;
		/*名前マップ。<タスク名,タスクID>*/
		NameMapType task_name_map_;
		/*管理処理リスト*/
		std::vector<std::function<void()>> management_process_list_;

		utility::WeakPointer<core::SceneData> scene_data_;
	public:
		Impl_(core::IGameAccessor& engine):game_(engine) {}
		//////////////////////////////////////////////////////////////////////////
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) {
			scene_data_ = scene_data_;
		}
		//////////////////////////////////////////////////////////////////////////
		/*タスク削除要請*/
		bool DisposeTaskRequest(TaskData& tdata) {
			if (tdata.state == TaskState::Disposed) { return false; }
			//削除処理を追加
			management_process_list_.push_back([&tdata, this] {
				DisposeTask(tdata);
			});
			tdata.state = TaskState::Disposed;
			return true;
		}
		/*タスク停止要請*/
		bool PauseTaskRequest(TaskData& tdata) {
			if (tdata.state != TaskState::Running) { return false; }
			//削除処理を追加
			management_process_list_.push_back([&tdata, this] {
				InctivateTask(tdata);
			});
			tdata.state = TaskState::Pausing;
			return true;
		}
		/*タスク再開要請*/
		bool ResumeTaskRequest(TaskData& tdata) {
			if (tdata.state != TaskState::Pausing) { return false; }
			//削除処理を追加
			management_process_list_.push_back([&tdata, this] {
				ActivateTask(tdata);
			});
			tdata.state = TaskState::Running;
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		/*タスクの有効化*/
		void ActivateTask(TaskData& tdata) {
			assert(tdata.state == TaskState::Running);
			running_task_group_list_[tdata.group_number].push_back(tdata.task.get());
			tdata.iterator_at_runnning_task_group = --running_task_group_list_[tdata.group_number].end();
		}
		/*タスクの無効化*/
		bool InctivateTask(TaskData& tdata) {
			assert(tdata.state == TaskState::Running);
			running_task_group_list_[tdata.group_number].erase(tdata.iterator_at_runnning_task_group);
			return true;
		}
		/*タスクの破棄*/
		bool DisposeTask(TaskData& tdata) {
			assert(tdata.state != TaskState::Disposed);
			if (tdata.state == TaskState::Running) { //実行中だったら無効化する。
				InctivateTask(tdata);
			}
			task_list_.erase(tdata.iterator_at_task_list); //タスクリストから削除
			if (tdata.is_named) {
				task_name_map_.erase(tdata.iterator_at_name_map); //名前マップから削除
			}
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		/*タスクの設定*/
		void SetupTask(const std::shared_ptr<TaskData> tdata, bool is_system_task) {
			//ここでラムダ関数がtdataのシェアポをキャプチャしておくことで、tdata使用中の解放を防ぐ。
			std::unique_ptr<core::TaskManagerConnection> manager_connection = std::make_unique<core::TaskManagerConnection>(
				[this, tdata] {return PauseTaskRequest(*tdata); }, //Pauser
				[this, tdata] {return ResumeTaskRequest(*tdata); }, //Resumer
				is_system_task ?
				std::function<void()>([] { debug::SystemLog::instance().Log(debug::LogLevel::Fatal, __FUNCTION__, "システムタスクを削除しようとしました。"); }) :
				[this, tdata] { DisposeTaskRequest(*tdata); } //Disposer(システムタスクの場合は削除できないDisposerをセット)
				);
			tdata->task->SystemSetUpAndInitialize(std::move(manager_connection), scene_data_);
		}
		/*タスクをマップに登録*/
		std::shared_ptr<TaskData> RegisterTaskToList(const std::shared_ptr<Task>& task, int group_number) {
			auto ptdata = std::make_shared<TaskData>();
			ptdata->task = task;
			ptdata->group_number = group_number;
			task_list_.push_back(ptdata);
			return ptdata;
		}
		/*名前を登録*/
		bool RegisterTaskName(const std::string& name, const std::shared_ptr<TaskData>& ptdata) {
			//名前マップに登録し、タスクデータにそのイテレータをセットする。
			ptdata->iterator_at_name_map = task_name_map_.emplace(name, ptdata).first;
			ptdata->is_named = true;
			return true;
		}
		/*管理処理Queを処理する*/
		void HandleManagementQue() {
			for (auto& prc : management_process_list_) {
				prc();
			}
			management_process_list_.clear();
		}

		std::shared_ptr<Task> GetTask(const std::string& name) {
			auto nit = task_name_map_.find(name);
			return nit == task_name_map_.end() ? nullptr : nit->second->task;
		}
		//有効なタスクのメンバ関数を実行
		void ExcuteValidTasksFunction(void(Task::* func)()) {
			for (int i = 0; i < TASK_SLOT_SIZE; ++i) {
				if (valid_slot_bitmap_[i]) { //有効なものだけ実行
					auto & tg = running_task_group_list_[i];
					for (auto& t : tg) {
						(t->*func)();
					}
				}
			}
		}
		//全削除
		void AllClear() {
			management_process_list_.clear();
			task_name_map_.clear();
			task_list_.clear();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//TaskManager
	//////////////////////////////////////////////////////////////////////////
	TaskManager::TaskManager(core::IGameAccessor& engine) :impl_(std::make_unique<Impl_>(engine)) {}

	TaskManager::~TaskManager() = default;

	void TaskManager::ExcuteTask() {
		impl_->ExcuteValidTasksFunction(&Task::Update);
	}

	void TaskManager::Update() {
		//管理処理を行う
		impl_->HandleManagementQue();
	}

	void TaskManager::Finalize() {
		impl_->HandleManagementQue(); //管理処理
		//まだ存在するプロセスの終了処理を行う
		impl_->ExcuteValidTasksFunction(&Task::Dispose);
		//全部クリア
		impl_->AllClear();
	}

	void TaskManager::SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) {
		impl_->SetSceneData(scene_data);
	}

	utility::WeakPointer<Task> TaskManager::GetTask(const std::string& name) {
		return impl_->GetTask(name);
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, false);
		return task;
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, false);
		return impl_->RegisterTaskName(name, ptdata) ? task : nullptr;
	}

	std::shared_ptr<Task> TaskManager::CreateSystemTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, core::SystemTaskSlot slot) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSystemSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, true);
		return task;
	}

}