#include "TaskManager.h"
#include "SceneAccessorForTask.h"
#include "Task.h"
#include "SceneData.h"
#include "SceneDataForTask.h"
#include "TaskManagerConnection.h"
#include "SystemLog.h"

namespace planeta_engine {

	TaskManager::TaskManager(core::IGameAccessor& engine) :game_(engine) {}

	TaskManager::~TaskManager() = default;

	void TaskManager::Update() {
		for (int i = 0; i < TASK_SLOT_SIZE; ++i) {
			if (valid_slot_bitmap_[i]) { //有効なものだけ実行
				auto & tg = task_group_list_[i];
				for (auto& t : tg) {
					t->Update();
				}
			}
		}
	}

	bool TaskManager::Process() {
		//登録解除リストのプロセスを削除する
		TaskDisposeList_();
		return true;
	}

	bool TaskManager::RemoveTask_(const InternalPosition& pos) {
		//登録解除リストに追加
		dispose_list_.push_back(pos);
		return true;
	}

	void TaskManager::Finalize() {
		TaskDisposeList_(); //破棄リスト処理
		//まだ存在するプロセスの終了処理を行う
		for (int i = 0; i < TASK_SLOT_SIZE; ++i) {
			if (valid_slot_bitmap_[i]) { //有効なものだけ実行
				auto & tg = task_group_list_[i];
				for (auto& t : tg) {
					t->Dispose();
				}
			}
		}
		//全部クリア
		dispose_list_.clear();
		task_name_id_map_.clear();
		task_id_map_.clear();
	}

	void TaskManager::SetupTask_(const std::shared_ptr<Task>& game_process, const InternalPosition& pos) {
		std::unique_ptr<core::TaskManagerConnection> manager_connection = std::make_unique<core::TaskManagerConnection> (
			[]() ->bool{}, //Pauser
			[]() ->bool {}, //Resumer
			[]() {}, //Disposer
			scene_accessor_
		);
		game_process->SystemSetUpAndInitialize(std::move(manager_connection), *scene_data_);
	}

	void TaskManager::SetSceneInterface(core::ScenePublicInterface& spi) {
		scene_accessor_ = std::make_shared<SceneAccessorForTask>(spi);
	}

	void TaskManager::SetSceneData(const core::SceneData& scene_data) {
		scene_data_ = std::make_unique<core::SceneDataForTask>();
	}

	bool TaskManager::RegisterToIDMap_(Task* id, const InternalPosition& pos_at_plist) {
		task_id_map_.emplace(id, pos_at_plist);
		return true;
	}

	void TaskManager::TaskDisposeList_() {
		for (auto& pos : dispose_list_) {
			auto& task_group = task_group_list_[pos.group_number];
			task_group.erase(pos.iterator_at_task_group);
		}
		dispose_list_.clear();
	}

	bool TaskManager::RegisterTaskName_(const std::string& name, Task* id) {
		task_name_id_map_.emplace(name, id);
		return true;
	}

	utility::WeakPointer<Task> TaskManager::GetProcess(const std::string& name) {
		auto nit = task_name_id_map_.find(name);
		if (nit == task_name_id_map_.end()) { return nullptr; }
		auto pit = task_id_map_.find(nit->second);
		if (pit == task_id_map_.end()) {
			task_name_id_map_.erase(nit);
			return nullptr;
		} else {
			return *pit->second.iterator_at_task_group;
		}
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot) {
		auto task = creator(game_);

		int group_number = GetGroupNumberFromSlot(slot);
		task_group_list_[group_number].push_back(task);
		InternalPosition pos(group_number, --task_group_list_[group_number].end());

		SetupTask_(task, pos);
		return RegisterToIDMap_(task.get(), pos) ? task : nullptr;
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name) {
		auto task = creator(game_);

		int group_number = GetGroupNumberFromSlot(slot);
		task_group_list_[group_number].push_back(task);
		InternalPosition pos(group_number, --task_group_list_[group_number].end());

		SetupTask_(task, pos);
		return (RegisterToIDMap_(task.get(), pos) && RegisterTaskName_(name, task.get())) ? task : nullptr;
	}

	int TaskManager::GetGroupNumberFromSlot(TaskSlot slot) {
		return slot_group_number_map_[static_cast<std::underlying_type_t<TaskSlot>>(slot)];
	}

}