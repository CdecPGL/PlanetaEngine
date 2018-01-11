#include <unordered_map>
#include <list>
#include <array>
#include <type_traits>
#include <bitset>
#include "planeta/core/StandardTaskManager.hpp"
#include "Task.hpp"
#include "TaskManagerConnection.hpp"
#include "planeta/core/LogUtility.hpp"
#include "SystemTaskSlot.hpp"
#include "IDebugManager.hpp"
#include "planeta/core/ISceneInternal.hpp"

namespace plnt {
	namespace private_ {
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
			constexpr int DRAW_GUI_SLOT = 15;
			constexpr int SLOT_COUNT = 16;
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
				APPCAM_SLOT,
				DRAW_SLOT,
				DRAW_GUI_SLOT,
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
		class StandardTaskManager::Impl_ {
		public:
			struct TaskData;
			//タスク群タイプ
			using TaskGroupType = std::list<Task*>;
			//タスクリストタイプ
			using TaskListType = std::list<std::shared_ptr<TaskData>>;
			//名前マップタイプ
			using NameMapType = std::unordered_map<std::string, std::shared_ptr<TaskData>>;
			/*タスクの状態(順に、実行中、停止中、破棄された)*/
			enum class TaskState { Running, Pausing, Disposed, PauseRequested, ResumeRequested, DisposeRequested };
			/*タスクの情報*/
			struct TaskData {
				TaskData() = default;
				TaskData(const TaskData&) = delete;
				TaskData& operator=(const TaskData&) = delete;
				std::shared_ptr<Task> task;
				TaskListType::iterator iterator_at_task_data_list;
				NameMapType::iterator iterator_at_task_name_map;
				bool is_named = false;
				int group_number; //所属しているタスクグループ番号
				TaskGroupType::iterator iterator_at_task_excution_list; //所属しているタスクグループでのイテレータ
				TaskState state = TaskState::Pausing;
			};
		private:
			//タスク群リスト
			std::array<TaskGroupType, SLOT_COUNT> task_excution_list_;

			/*タスクリスト*/
			TaskListType task_data_list_;
			/*名前マップ。<タスク名,タスクID>*/
			NameMapType task_name_map_;
			/*管理処理リスト*/
			std::vector<std::function<void()>> management_process_list_;
			/**/
			bool is_system_task_disposable_ = false; //システムタスクが削除可能か。
		public:
			Impl_() {}
			//システムタスク削除の有効化
			void ValidateSystemTaskDisposal() { is_system_task_disposable_ = true; }
			//////////////////////////////////////////////////////////////////////////
			/*タスク削除要請*/
			bool DisposeTaskRequest(TaskData& tdata, bool is_system_task) {
				//実行中、停止中、実行再開依頼中、停止依頼中に有効
				if (tdata.state == TaskState::Disposed ||
					tdata.state == TaskState::DisposeRequested) {
					return false;
				}
				//システムタスクの削除をチェック
				if (is_system_task == true && is_system_task_disposable_ == false) {
					PE_LOG_ERROR("システムタスクの削除は許可されていません。");
				}
				//削除処理を追加
				switch (tdata.state) {
				case TaskState::Running:
				case TaskState::ResumeRequested:
					management_process_list_.push_back([&tdata, this] {
						InctivateTask(tdata);
						DisposeTask(tdata);
					});
					break;
				case TaskState::Pausing:
				case TaskState::PauseRequested:
					management_process_list_.push_back([&tdata, this] {
						DisposeTask(tdata);
					});
					break;
				default:
					assert(false);
					return false;
				}
				tdata.state = TaskState::DisposeRequested;
				return true;
			}
			/*タスク停止要請*/
			bool PauseTaskRequest(TaskData& tdata) {
				//実行中か、実行再開依頼が発行されている場合にのみ有効
				if (tdata.state == TaskState::Disposed ||
					tdata.state == TaskState::DisposeRequested ||
					tdata.state == TaskState::Pausing ||
					tdata.state == TaskState::PauseRequested
					) {
					return false;
				}
				//停止処理を追加
				management_process_list_.push_back([&tdata, this] {
					InctivateTask(tdata);
				});
				tdata.state = TaskState::PauseRequested;
				return true;
			}
			/*タスク再開要請*/
			bool ResumeTaskRequest(TaskData& tdata) {
				//停止か、停止依頼が発行されている場合にのみ有効
				if (tdata.state == TaskState::Disposed ||
					tdata.state == TaskState::DisposeRequested ||
					tdata.state == TaskState::Running ||
					tdata.state == TaskState::ResumeRequested
					) {
					return false;
				}
				//再開処理を追加
				/*management_process_list_.push_back([&tdata, this] {
				ActivateTask(tdata);
				});*/
				tdata.state = TaskState::ResumeRequested;
				//再開処理はすぐに実行する。(実行リストにはstd::listを用いており、ループ中に末尾に追加されても問題ないため)
				ActivateTask(tdata);
				return true;
			}
			//////////////////////////////////////////////////////////////////////////
			/*タスクの有効化*/
			void ActivateTask(TaskData& tdata) {
				task_excution_list_[tdata.group_number].push_back(tdata.task.get());
				tdata.iterator_at_task_excution_list = --task_excution_list_[tdata.group_number].end();
				tdata.state = TaskState::Running;
			}
			/*タスクの無効化*/
			bool InctivateTask(TaskData& tdata) {
				task_excution_list_[tdata.group_number].erase(tdata.iterator_at_task_excution_list);
				tdata.state = TaskState::Pausing;
				return true;
			}
			/*タスクの破棄*/
			bool DisposeTask(TaskData& tdata) {
				task_data_list_.erase(tdata.iterator_at_task_data_list); //タスクリストから削除
				if (tdata.is_named) {
					task_name_map_.erase(tdata.iterator_at_task_name_map); //名前マップから削除
				}
				if (tdata.state == TaskState::Running) {
					task_excution_list_[tdata.group_number].erase(tdata.iterator_at_task_excution_list); //タスクグループリストから削除
				}
				tdata.state = TaskState::Disposed;
				return true;
			}
			//////////////////////////////////////////////////////////////////////////
			/*タスクの設定*/
			void SetupTask(const std::shared_ptr<TaskData> tdata, bool is_system_task, const WeakPointer<IScene>& i_scene) {
				//ここでラムダ関数がtdataのシェアポをキャプチャしておくことで、tdata使用中の解放を防ぐ。
				std::unique_ptr<private_::TaskManagerConnection> manager_connection = std::make_unique<private_::TaskManagerConnection>(
					[this, tdata] {return PauseTaskRequest(*tdata); }, //Pauser
					[this, tdata] {return ResumeTaskRequest(*tdata); }, //Resumer
					is_system_task ?
					std::function<void()>([this, tdata] { DisposeTaskRequest(*tdata, true); }) :
					[this, tdata] { DisposeTaskRequest(*tdata, false); } //Disposer(システムタスクの場合は削除できないDisposerをセット)
				);
				tdata->task->SystemSetUpAndInitialize(std::move(manager_connection), i_scene);
			}
			/*タスクをマップに登録*/
			std::shared_ptr<TaskData> RegisterTaskToList(const std::shared_ptr<Task>& task, int group_number, bool auto_run) {
				auto ptdata = std::make_shared<TaskData>();
				ptdata->task = task;
				ptdata->group_number = group_number;
				ptdata->is_named = false;
				ptdata->state = TaskState::Pausing;
				//タスクデータリストに登録して、自身を指すイテレータを保持
				task_data_list_.push_back(ptdata);
				ptdata->iterator_at_task_data_list = --task_data_list_.end();
				//タスクグループリストに登録して、自身を指すイテレータを保持
				if (auto_run) {
					ptdata->state = TaskState::Running;
					task_excution_list_[group_number].push_back(task.get());
					ptdata->iterator_at_task_excution_list = --task_excution_list_[group_number].end();
				}

				return ptdata;
			}
			/*名前を登録*/
			bool RegisterTaskName(const std::string& name, const std::shared_ptr<TaskData>& ptdata) {
				//名前マップに登録し、タスクデータにそのイテレータをセットする。
				ptdata->iterator_at_task_name_map = task_name_map_.emplace(name, ptdata).first;
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
					auto & tg = task_excution_list_[i];
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
				for (auto&& tg : task_excution_list_) {
					tg.clear();
				}
			}
			//タスク数を数える
			void CountTask(int& active_task_count, int& inactive_task_count) {
				active_task_count = 0;
				inactive_task_count = 0;
				for (auto&& td : task_data_list_) {
					switch (td->state) {
					case TaskState::Running:
					case TaskState::PauseRequested:
						++active_task_count;
						break;
					case TaskState::Pausing:
					case TaskState::ResumeRequested:
						++inactive_task_count;
						break;
					case TaskState::Disposed:
					case TaskState::DisposeRequested:
					default:
						break;

					}
				}
			}
		};

		//////////////////////////////////////////////////////////////////////////
		//TaskManager
		//////////////////////////////////////////////////////////////////////////
		StandardTaskManager::StandardTaskManager() :impl_(std::make_unique<Impl_>()) {}

		StandardTaskManager::~StandardTaskManager() = default;

		void StandardTaskManager::ExcuteTask() {
			impl_->ExcuteValidTasksFunction(&Task::Update);
		}

		void StandardTaskManager::Update() {
			//管理処理を行う
			impl_->HandleManagementQue();
		}

		void StandardTaskManager::Finalize() {
			impl_->HandleManagementQue(); //管理処理
			impl_->ValidateSystemTaskDisposal(); //システムタスクを削除可能に。
			//まだ存在するプロセスの終了処理を行う
			impl_->ExcuteValidTasksFunction(&Task::Dispose);
			//全部クリア
			impl_->AllClear();
		}

		WeakPointer<Task> StandardTaskManager::GetTask(const std::string& name)const {
			return impl_->GetTask(name);
		}

		bool StandardTaskManager::RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, bool auto_run) {
			int group_number = GetGroupNumberFromSlot(slot);
			auto ptdata = impl_->RegisterTaskToList(task, group_number, auto_run);
			impl_->SetupTask(ptdata, false, scene_internal_interface());
			return task != nullptr;
		}

		bool StandardTaskManager::RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name, bool auto_run) {
			int group_number = GetGroupNumberFromSlot(slot);
			auto ptdata = impl_->RegisterTaskToList(task, group_number, auto_run);
			impl_->SetupTask(ptdata, false, scene_internal_interface());
			return impl_->RegisterTaskName(name, ptdata);
		}

		std::shared_ptr<Task> StandardTaskManager::RegisterSystemTask(const std::shared_ptr<Task>& task, private_::SystemTaskSlot slot) {
			int group_number = GetGroupNumberFromSystemSlot(slot);
			auto ptdata = impl_->RegisterTaskToList(task, group_number, true);
			impl_->SetupTask(ptdata, true, scene_internal_interface());
			return task;
		}

		void StandardTaskManager::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			di_adder.AddLine("-----TaskManager-----");
			int active_task{ 0 }, inactive_task{ 0 };
			impl_->CountTask(active_task, inactive_task);
			di_adder.AddLineV("実行中タスク数:", active_task);
			di_adder.AddLineV("停止中タスク数:", inactive_task);
		}
	}
}