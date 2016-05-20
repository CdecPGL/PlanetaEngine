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
		/*����������*/
		bool Initialize()override { return true; }
		/*�I������*/
		void Finalize()override;
		/*�V�[���C���^�[�t�F�C�X���Z�b�g*/
		void SetSceneInterface(core::ScenePublicInterface& spi)override;
		/*�V�[���f�[�^���Z�b�g*/
		void SetSceneData(const core::SceneData& scene_data)override;

		/*�v���Z�X�̃A�b�v�f�[�g*/
		void Update();
		/*�Ǘ�����*/
		bool Process();

		/*�V�X�e���v���Z�X�ǉ�(�V�X�e���v���Z�X��ID�}�b�v�▼�O�}�b�v�ɓo�^����Ȃ����߁A�폜�s�\)*/
		template<class C>
		utility::WeakPointer<C> AddSystemProcess(const core::TaskPosition& pos) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
			auto new_pos = AddTaskToList_(new_proc, pos);
			SetupTask_(new_proc, []() {return true; }, core::TaskPosition(new_pos.iterator_at_priority_list));
			return new_proc;
		}

		/*���O����Q�[���v���Z�X���擾*/
		utility::WeakPointer<Task> GetProcess(const std::string& name)override;
		/*�Q�[���v���Z�X�쐻*/
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
		//�^�X�N�Q�^�C�v
		using TaskGroup = std::list<std::shared_ptr<Task>>;
		//�X���b�g�}�b�v
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
		//�^�X�N�Q���X�g
		std::array<TaskGroup,SLOT_COUNT> task_group_list_;
		//�L���X���b�g�r�b�g�}�b�v
		std::bitset<SLOT_COUNT> valid_slot_bitmap_;

		/*�����ʒu*/
		struct InternalPosition {
			InternalPosition(int gn,const TaskGroup::iterator& it) :group_number(gn), iterator_at_task_group(it) {}
			int group_number;
			TaskGroup::iterator iterator_at_task_group;
		};
		/*ID�}�b�v�B<id,���X�g���ʒu>*/
		using TaskIDMapType = std::unordered_map<Task*, InternalPosition>;
		TaskIDMapType task_id_map_;
		/*���OID�}�b�v�B<�^�X�N��,ID>*/
		using TaskNameMapType = std::unordered_map<std::string, Task*>;
		TaskNameMapType task_name_id_map_;
		/*�o�^�������X�g*/
		std::vector<InternalPosition> dispose_list_;

		/*�v���Z�X�̐ݒ�*/
		void SetupTask_(const std::shared_ptr<Task>& game_process, const InternalPosition& pos);
		/*�^�X�N�X���b�g����^�X�N�O���[�v�ԍ����擾*/
		int GetGroupNumberFromSlot(TaskSlot slot);
		/*���O��o�^*/
		bool RegisterTaskName_(const std::string& name, Task* id);
		/*ID�}�b�v�Ƀv���Z�X�o�^*/
		bool RegisterToIDMap_(Task* id, const InternalPosition& pos_at_plist);
		/*�ʒu�Ńv���Z�X�폜*/
		bool RemoveTask_(const InternalPosition& pos);
		/*�j�����X�g����������*/
		void TaskDisposeList_();

		core::IGameAccessor& game_;
		std::shared_ptr<SceneAccessorForTask> scene_accessor_;
		std::unique_ptr<core::SceneDataForTask> scene_data_;
	};
}