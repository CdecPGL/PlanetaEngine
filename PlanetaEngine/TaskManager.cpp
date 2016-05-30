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
		constexpr int TASK_SLOT_1 = 1;
		constexpr int APPVEL_SLOT = 2;
		constexpr int COLDET_SLOT = 3;
		constexpr int TASK_SLOT_2 = 4;
		constexpr int TASK_SLOT_3 = 5;
		constexpr int TASK_SLOT_4 = 6;
		constexpr int TASK_SLOT_5 = 7;
		constexpr int APPCAM_SLOT = 8;
		constexpr int TASK_SLOT_6 = 9;
		constexpr int TASK_SLOT_7 = 10;
		constexpr int TASK_SLOT_8 = 11;
		constexpr int TASK_SLOT_9 = 12;
		constexpr int DRAW_SLOT = 13;
		constexpr int SLOT_COUNT = 14;
		//�X���b�g�}�b�v
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
		};
		//�V�X�e���X���b�g�}�b�v
		using namespace core;
		constexpr std::array<int, SYSTEM_TASK_SLOT_SIZE> system_slot_group_number_map_ = {
			APPVEL_SLOT,
			COLDET_SLOT,
			APPVEL_SLOT,
			DRAW_SLOT,
		};
		/*�^�X�N�X���b�g����^�X�N�O���[�v�ԍ����擾*/
		int GetGroupNumberFromSlot(TaskSlot slot) {
			return slot_group_number_map_[static_cast<std::underlying_type_t<TaskSlot>>(slot)];
		}
		/*�V�X�e���^�X�N�X���b�g����^�X�N�O���[�v�ԍ����擾*/
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
		//�^�X�N�Q�^�C�v
		using TaskGroupType = std::list<Task*>;
		//�^�X�N���X�g�^�C�v
		using TaskListType = std::list<std::shared_ptr<TaskData>>;
		//���O�}�b�v�^�C�v
		using NameMapType = std::unordered_map<std::string, std::shared_ptr<TaskData>>;
		/*�^�X�N�̏��(���ɁA���s���A��~���A�j�����ꂽ)*/
		enum class TaskState { Running, Pausing, Disposed };
		/*�^�X�N�̏��*/
		struct TaskData {
			TaskData() = default;
			TaskData(const TaskData&) = delete;
			TaskData& operator=(const TaskData&) = delete;
			std::shared_ptr<Task> task;
			TaskListType::iterator iterator_at_task_list;
			NameMapType::iterator iterator_at_name_map;
			bool is_named = false;
			int group_number; //�������Ă���^�X�N�O���[�v�ԍ�
			TaskGroupType::iterator iterator_at_task_group; //�������Ă���^�X�N�O���[�v�ł̃C�e���[�^
			TaskState state = TaskState::Pausing;
		};
	private:
		//�^�X�N�Q���X�g
		std::array<TaskGroupType, SLOT_COUNT> task_group_list_;

		/*�^�X�N���X�g*/
		TaskListType task_data_list_;
		/*���O�}�b�v�B<�^�X�N��,�^�X�NID>*/
		NameMapType task_name_map_;
		/*�Ǘ��������X�g*/
		std::vector<std::function<void()>> management_process_list_;
		/**/
		bool is_system_task_disposable_ = false; //�V�X�e���^�X�N���폜�\���B

		utility::WeakPointer<core::SceneData> scene_data_;
	public:
		Impl_(){}
		//�V�X�e���^�X�N�폜�̗L����
		void ValidateSystemTaskDisposal() { is_system_task_disposable_ = true; }
		//////////////////////////////////////////////////////////////////////////
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) {
			scene_data_ = scene_data_;
		}
		//////////////////////////////////////////////////////////////////////////
		/*�^�X�N�폜�v��*/
		bool DisposeTaskRequest(TaskData& tdata, bool is_system_task) {
			if (tdata.state == TaskState::Disposed) { return false; }
			//�V�X�e���^�X�N�̍폜���`�F�b�N
			if (is_system_task == true && is_system_task_disposable_ == false) {
				PE_LOG_ERROR("�V�X�e���^�X�N�̍폜�͋�����Ă��܂���B");
			}
			//�폜������ǉ�
			management_process_list_.push_back([&tdata, this] {
				DisposeTask(tdata);
			});
			tdata.state = TaskState::Disposed;
			return true;
		}
		/*�^�X�N��~�v��*/
		bool PauseTaskRequest(TaskData& tdata) {
			if (tdata.state != TaskState::Running) { return false; }
			//�폜������ǉ�
			management_process_list_.push_back([&tdata, this] {
				InctivateTask(tdata);
			});
			tdata.state = TaskState::Pausing;
			return true;
		}
		/*�^�X�N�ĊJ�v��*/
		bool ResumeTaskRequest(TaskData& tdata) {
			if (tdata.state != TaskState::Pausing) { return false; }
			//�폜������ǉ�
			management_process_list_.push_back([&tdata, this] {
				ActivateTask(tdata);
			});
			tdata.state = TaskState::Running;
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		/*�^�X�N�̗L����*/
		void ActivateTask(TaskData& tdata) {
			assert(tdata.state == TaskState::Running);
			task_group_list_[tdata.group_number].push_back(tdata.task.get());
			tdata.iterator_at_task_group = --task_group_list_[tdata.group_number].end();
		}
		/*�^�X�N�̖�����*/
		bool InctivateTask(TaskData& tdata) {
			PE_VERIFY(tdata.state == TaskState::Running);
			task_group_list_[tdata.group_number].erase(tdata.iterator_at_task_group);
			return true;
		}
		/*�^�X�N�̔j��*/
		bool DisposeTask(TaskData& tdata) {
			PE_VERIFY(tdata.state != TaskState::Disposed);
			if (tdata.state == TaskState::Running) { //���s���������疳��������B
				InctivateTask(tdata);
			}
			task_data_list_.erase(tdata.iterator_at_task_list); //�^�X�N���X�g����폜
			if (tdata.is_named) {
				task_name_map_.erase(tdata.iterator_at_name_map); //���O�}�b�v����폜
			}
			task_group_list_[tdata.group_number].erase(tdata.iterator_at_task_group); //�^�X�N�O���[�v���X�g����폜
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		/*�^�X�N�̐ݒ�*/
		void SetupTask(const std::shared_ptr<TaskData> tdata, bool is_system_task) {
			//�����Ń����_�֐���tdata�̃V�F�A�|���L���v�`�����Ă������ƂŁAtdata�g�p���̉����h���B
			std::unique_ptr<core::TaskManagerConnection> manager_connection = std::make_unique<core::TaskManagerConnection>(
				[this, tdata] {return PauseTaskRequest(*tdata); }, //Pauser
				[this, tdata] {return ResumeTaskRequest(*tdata); }, //Resumer
				is_system_task ?
				std::function<void()>([this, tdata] { DisposeTaskRequest(*tdata, true); }) :
				[this, tdata] { DisposeTaskRequest(*tdata,false); } //Disposer(�V�X�e���^�X�N�̏ꍇ�͍폜�ł��Ȃ�Disposer���Z�b�g)
				);
			tdata->task->SystemSetUpAndInitialize(std::move(manager_connection), scene_data_);
		}
		/*�^�X�N���}�b�v�ɓo�^*/
		std::shared_ptr<TaskData> RegisterTaskToList(const std::shared_ptr<Task>& task, int group_number) {
			auto ptdata = std::make_shared<TaskData>();
			ptdata->task = task;
			ptdata->group_number = group_number;
			task_data_list_.push_back(ptdata);
			task_group_list_[group_number].push_back(task.get());
			return ptdata;
		}
		/*���O��o�^*/
		bool RegisterTaskName(const std::string& name, const std::shared_ptr<TaskData>& ptdata) {
			//���O�}�b�v�ɓo�^���A�^�X�N�f�[�^�ɂ��̃C�e���[�^���Z�b�g����B
			ptdata->iterator_at_name_map = task_name_map_.emplace(name, ptdata).first;
			ptdata->is_named = true;
			return true;
		}
		/*�Ǘ�����Que����������*/
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
		//�L���ȃ^�X�N�̃����o�֐������s
		void ExcuteValidTasksFunction(void(Task::* func)()) {
			for (int i = 0; i < SLOT_COUNT; ++i) {
				auto & tg = task_group_list_[i];
				for (auto& t : tg) {
					(t->*func)();
				}
			}
		}
		//�S�폜
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
		//�Ǘ��������s��
		impl_->HandleManagementQue();
	}

	void TaskManager::Finalize() {
		impl_->HandleManagementQue(); //�Ǘ�����
		impl_->ValidateSystemTaskDisposal(); //�V�X�e���^�X�N���폜�\�ɁB
		//�܂����݂���v���Z�X�̏I���������s��
		impl_->ExcuteValidTasksFunction(&Task::Dispose);
		//�S���N���A
		impl_->AllClear();
	}

	void TaskManager::SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) {
		impl_->SetSceneData(scene_data);
	}

	utility::WeakPointer<Task> TaskManager::GetTask(const std::string& name)const {
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

	std::shared_ptr<Task> TaskManager::RegisterSystemTask(const std::shared_ptr<Task>& task, core::SystemTaskSlot slot) {
		int group_number = GetGroupNumberFromSystemSlot(slot);
		auto ptdata = impl_->RegisterTaskToList(task, group_number);
		impl_->SetupTask(ptdata, true);
		return task;
	}

}