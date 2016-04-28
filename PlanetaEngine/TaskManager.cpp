#include <unordered_map>
#include <list>
#include <array>
#include <type_traits>
#include <bitset>
#include "TaskManager.h"
#include "SceneAccessorForTask.h"
#include "Task.h"
#include "SceneData.h"
#include "SceneDataForTask.h"
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
		constexpr int UDTANM_SLOT = 14;
		constexpr int TASK_SLOT_6 = 15;
		constexpr int COVCON_SLOT_6 = 16;
		constexpr int TASK_SLOT_7 = 17;
		constexpr int COVCON_SLOT_7 = 18;
		constexpr int DRAW_SLOT = 19;
		constexpr int SLOT_COUNT = 20;
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
			UDTANM_SLOT,
			COVCON_SLOT_6,
			COVCON_SLOT_7,
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
		core::IGameAccessor& game_;
		//タスク群タイプ
		using TaskGroup = std::list<std::shared_ptr<Task>>;
		/*内部位置*/
		struct InternalPosition {
			InternalPosition(int gn, const TaskGroup::iterator& it) :group_number(gn), iterator_at_task_group(it) {}
			int group_number;
			TaskGroup::iterator iterator_at_task_group;
		};
	private:
		//タスク群リスト
		std::array<TaskGroup, SLOT_COUNT> task_group_list_;
		//有効スロットビットマップ
		std::bitset<SLOT_COUNT> valid_slot_bitmap_;

		/*IDマップ。<id,リスト内位置>*/
		using TaskIDMapType = std::unordered_map<Task*, InternalPosition>;
		TaskIDMapType task_id_map_;
		/*名前IDマップ。<タスク名,ID>*/
		using TaskNameMapType = std::unordered_map<std::string, Task*>;
		TaskNameMapType task_name_id_map_;
		/*登録解除リスト*/
		std::vector<InternalPosition> dispose_list_;

		std::shared_ptr<SceneAccessorForTask> scene_accessor_;
		std::unique_ptr<core::SceneDataForTask> scene_data_;
	public:
		Impl_(core::IGameAccessor& engine):game_(engine) {}

		void SetSceneInterface(core::ScenePublicInterface& spi) {
			scene_accessor_ = std::make_shared<SceneAccessorForTask>(spi);
		}

		void SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForTask>();
		}

		/*プロセスの設定*/
		void SetupTask_(const std::shared_ptr<Task>& game_process, const InternalPosition& pos) {
			std::unique_ptr<core::TaskManagerConnection> manager_connection = std::make_unique<core::TaskManagerConnection>(
				[]() ->bool {}, //Pauser
				[]() ->bool {}, //Resumer
				[]() {}, //Disposer
				scene_accessor_
				);
			game_process->SystemSetUpAndInitialize(std::move(manager_connection), *scene_data_);
		}
		/*タスクをリストに登録*/
		InternalPosition RegisterTaskToList(const std::shared_ptr<Task>& task, int group_number) {
			task_group_list_[group_number].push_back(task);
			return InternalPosition(group_number, --task_group_list_[group_number].end());
		}
		/*名前を登録*/
		bool RegisterTaskName(const std::string& name, Task* id) {
			task_name_id_map_.emplace(name, id);
			return true;
		}
		/*IDマップにプロセス登録*/
		bool RegisterToIDMap(Task* id, const InternalPosition& pos_at_plist) {
			task_id_map_.emplace(id, pos_at_plist);
			return true;
		}
		/*位置でプロセス削除*/
		bool RemoveTask(const InternalPosition& pos) {
			//登録解除リストに追加
			dispose_list_.push_back(pos);
			return true;
		}
		/*破棄リストを処理する*/
		void ProcessDisposeList() {
			for (auto& pos : dispose_list_) {
				auto& task_group = task_group_list_[pos.group_number];
				task_group.erase(pos.iterator_at_task_group);
			}
			dispose_list_.clear();
		}

		std::shared_ptr<Task> GetTask(const std::string& name) {
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
		//有効なタスクのメンバ関数を実行
		void ExcuteValidTasksFunction(void(Task::* func)()) {
			for (int i = 0; i < TASK_SLOT_SIZE; ++i) {
				if (valid_slot_bitmap_[i]) { //有効なものだけ実行
					auto & tg = task_group_list_[i];
					for (auto& t : tg) {
						(t.get()->*func)();
					}
				}
			}
		}
		//全削除
		void AllClear() {
			dispose_list_.clear();
			task_name_id_map_.clear();
			task_id_map_.clear();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//TaskManager
	//////////////////////////////////////////////////////////////////////////
	TaskManager::TaskManager(core::IGameAccessor& engine) :impl_(std::make_unique<Impl_>(engine)) {}

	TaskManager::~TaskManager() = default;

	void TaskManager::Update() {
		impl_->ExcuteValidTasksFunction(&Task::Update);
	}

	bool TaskManager::Process() {
		//登録解除リストのプロセスを削除する
		impl_->ProcessDisposeList();
		return true;
	}

	void TaskManager::Finalize() {
		impl_->ProcessDisposeList(); //破棄リスト処理
		//まだ存在するプロセスの終了処理を行う
		impl_->ExcuteValidTasksFunction(&Task::Dispose);
		//全部クリア
		impl_->AllClear();
	}

	void TaskManager::SetSceneInterface(core::ScenePublicInterface& spi) {
		impl_->SetSceneInterface(spi);
	}

	void TaskManager::SetSceneData(const core::SceneData& scene_data) {
		impl_->SetSceneData(scene_data);
	}

	utility::WeakPointer<Task> TaskManager::GetTask(const std::string& name) {
		return impl_->GetTask(name);
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSlot(slot);
		Impl_::InternalPosition pos = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask_(task, pos);
		return impl_->RegisterToIDMap(task.get(), pos) ? task : nullptr;
	}

	std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSlot(slot);
		Impl_::InternalPosition pos = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask_(task, pos);
		return (impl_->RegisterToIDMap(task.get(), pos) && impl_->RegisterTaskName(name, task.get())) ? task : nullptr;
	}

	std::shared_ptr<Task> TaskManager::CreateSystemTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, core::SystemTaskSlot slot) {
		auto task = creator(impl_->game_);
		int group_number = GetGroupNumberFromSystemSlot(slot);
		Impl_::InternalPosition pos = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask_(task, pos);
		return task;
	}

}