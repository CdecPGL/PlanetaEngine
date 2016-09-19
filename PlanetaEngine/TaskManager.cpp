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

namespace planeta {
	namespace {
		constexpr int TASK_SLOT_0 = 0;
		constexpr int TASK_SLOT_1 = 1;
		constexpr int TASK_SLOT_2 = 2;
		constexpr int APPVEL_SLOT = 3;
		constexpr int COLDET_SLOT = 4;
		constexpr int TASK_SLOT_3 = 5;
		constexpr int TASK_SLOT_4 = 6;
		constexpr int TASK_SLOT_5 = 7;
		constexpr int TASK_SLOT_6 = 8;
		constexpr int APPCAM_SLOT = 9;
		constexpr int TASK_SLOT_7 = 10;
		constexpr int TASK_SLOT_8 = 11;
		constexpr int TASK_SLOT_9 = 12;
		constexpr int TASK_SLOT_10 = 13;
		constexpr int DRAW_SLOT = 14;
		constexpr int SLOT_COUNT = 15;
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
			TASK_SLOT_9,
			TASK_SLOT_10,
		};
		//システムスロットマップ
		using namespace private_;
		constexpr std::array<int, SYSTEM_TASK_SLOT_SIZE> system_slot_group_number_map_ = {
			APPVEL_SLOT,
			COLDET_SLOT,
			APPVEL_SLOT,
			DRAW_SLOT,
		};
		/*タスクスロットからタスクグループ番号を取得*/
		int GetGroupNumberFromSlot(TaskSlot slot) {
			return slot_group_number_map_[static_cast<std::underlying_type_t<TaskSlot>>(slot)];
		}
		/*システムタスクスロットからタスクグループ番号を取得*/
		int GetGroupNumberFromSystemSlot(private_::SystemTaskSlot slot) {
			return system_slot_group_number_map_[static_cast<std::underlying_type_t<private_::SystemTaskSlot>>(slot)];
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class TaskManager::Impl_ {
	public:
		struct TaskData;
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
			TaskGroupType::iterator iterator_at_task_group; //所属しているタスクグループでのイテレータ
			TaskState state = TaskState::Pausing;
		};
	private:
		//タスク群リスト
		std::array<TaskGroupType, SLOT_COUNT> task_group_list_;

		/*タスクリスト*/
		TaskListType task_data_list_;
		/*名前マップ。<タスク名,タスクID>*/
		NameMapType task_name_map_;
		/*管理処理リスト*/
		std::vector<std::function<void()>> management_process_list_;
		/**/
		bool is_system_task_disposable_ = false; //システムタスクが削除可能か。

		WeakPointer<private_::SceneData> scene_data_;
	public:
		Impl_(){}
		//システムタスク削除の有効化
		void ValidateSystemTaskDisposal() { is_system_task_disposable_ = true; }
		//////////////////////////////////////////////////////////////////////////
		void SetSceneData(const WeakPointer<private_::SceneData>& scene_data) {
			scene_data_ = scene_data_;
		}
		//////////////////////////////////////////////////////////////////////////
		/*タスク削除要請*/
		bool DisposeTaskRequest(TaskData& tdata, bool is_system_task) {
			if (tdata.state == TaskState::Disposed) { return false; }
			//システムタスクの削除をチェック
			if (is_system_task == true && is_system_task_disposable_ == false) {
				PE_LOG_ERROR("システムタスクの削除は許可されていません。");
			}
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
			task_group_list_[tdata.group_number].push_back(tdata.task.get());
			tdata.iterator_at_task_group = --task_group_list_[tdata.group_number].end();
		}
		/*タスクの無効化*/
		bool InctivateTask(TaskData& tdata) {
			PE_VERIFY(tdata.state == TaskState::Running);
			task_group_list_[tdata.group_number].erase(tdata.iterator_at_task_group);
			return true;
		}
		/*タスクの破棄*/
		bool DisposeTask(TaskData& tdata) {
			PE_VERIFY(tdata.state != TaskState::Disposed);
			if (tdata.state == TaskState::Running) { //実行中だったら無効化する。
				InctivateTask(tdata);
			}
			task_data_list_.erase(tdata.iterator_at_task_list); //タスクリストから削除
			if (tdata.is_named) {
				task_name_map_.erase(tdata.iterator_at_name_map); //名前マップから削除
			}
			task_group_list_[tdata.group_number].erase(tdata.iterator_at_task_group); //タスクグループリストから削除
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		/*タスクの設定*/
		void SetupTask(const std::shared_ptr<TaskData> tdata, bool is_system_task) {
			//ここでラムダ関数がtdataのシェアポをキャプチャしておくことで、tdata使用中の解放を防ぐ。
			std::unique_ptr<private_::TaskManagerConnection> manager_connection = std::make_unique<private_::TaskManagerConnection>(
				[this, tdata] {return PauseTaskRequest(*tdata); }, //Pauser
				[this, tdata] {return ResumeTaskRequest(*tdata); }, //Resumer
				is_system_task ?
				std::function<void()>([this, tdata] { DisposeTaskRequest(*tdata, true); }) :
				[this, tdata] { DisposeTaskRequest(*tdata,false); } //Disposer(システムタスクの場合は削除できないDisposerをセット)
				);
			tdata->task->SystemSetUpAndInitialize(std::move(manager_connection), scene_data_);
		}
		/*タスクをマップに登録*/
		std::shared_ptr<TaskData> RegisterTaskToList(const std::shared_ptr<Task>& task, int group_number) {
			auto ptdata = std::make_shared<TaskData>();
			ptdata->task = task;
			ptdata->group_number = group_number;
			task_data_list_.push_back(ptdata);
			task_group_list_[group_number].push_back(task.get());
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
			for (int i = 0; i < SLOT_COUNT; ++i) {
				auto & tg = task_group_list_[i];
				for (auto& t : tg) {
					(t->*func)();
				}
			}
		}
		//全削除
		void AllClear() {
			management_process_list_.clear();
			task_name_map_.clear();
			task_data_list_.clear();
			for (auto&& tg : task_group_list_) {
				tg.clear();
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//TaskManager
	//////////////////////////////////////////////////////////////////////////
	TaskManager::TaskManager() :impl_(std::make_unique<Impl_>()) {}

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
		impl_->ValidateSystemTaskDisposal(); //システムタスクを削除可能に。
		//まだ存在するプロセスの終了処理を行う
		impl_->ExcuteValidTasksFunction(&Task::Dispose);
		//全部クリア
		impl_->AllClear();
	}

	void TaskManager::SetSceneData(const WeakPointer<private_::SceneData>& scene_data) {
		impl_->SetSceneData(scene_data);
	}

	WeakPointer<Task> TaskManager::GetTask(const std::string& name)const {
		return impl_->GetTask(name);
	}

	bool TaskManager::RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot) {
		int group_number = GetGroupNumberFromSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, false);
		return task != nullptr;
	}

	bool TaskManager::RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name) {
		int group_number = GetGroupNumberFromSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, false);
		return impl_->RegisterTaskName(name, ptdata);
	}

	std::shared_ptr<Task> TaskManager::RegisterSystemTask(const std::shared_ptr<Task>& task, private_::SystemTaskSlot slot) {
		int group_number = GetGroupNumberFromSystemSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, true);
		return task;
	}

}