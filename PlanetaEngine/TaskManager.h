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
				SetupProcess_(new_proc, []() {return true; }, core::TaskPosition(new_pos.iterator_at_priority_list));
				return new_proc;
			}
			/*�L�[�|�W�V�����̐ݒ�B(�n���ꂽID�̏��Ԓʂ�ɍ쐬�����B�����̃L�[�|�W�V�����͍폜�����)*/
			bool SetKeyPositions(const std::list<std::string>& key_position_ids);

			/*���O����Q�[���v���Z�X���擾*/
			utility::WeakPointer<Task> GetProcess(const std::string& name)override;
			/*�Q�[���v���Z�X�쐻*/
			std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos)override;
			std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos, const std::string& name)override;
			/*�L�[�|�W�V�������擾����*/
			core::TaskPosition GetKeyPosition(const std::string& id)const override;
		private:
			/*�Q�[���v���Z�X���X�g
			�v�f�̒ǉ��폜�ŃC�e���[�^�������ɂȂ�Ȃ����Ƃ���list���g�p*/
			core::TaskPriorytyListType process_priority_list_;
			/*�����ʒu�ʒu*/
			struct InternalPosition {
				InternalPosition(const core::TaskPriorytyListType::iterator& pl_it, const core::TaskListType::iterator& l_it) :iterator_at_priority_list(pl_it), iterator_at_list(l_it) {}
				core::TaskPriorytyListType::iterator iterator_at_priority_list;
				core::TaskListType::iterator iterator_at_list;
			};
			/*ID�}�b�v�B<id,���X�g���ʒu>*/
			using ProcessIDMapType = std::unordered_map<Task*, InternalPosition>;
			ProcessIDMapType process_id_map_;
			/*���OID�}�b�v�B<�v���Z�X��,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, Task*>;
			ProcessNameMapType process_name_id_map_;
			/*�o�^�������X�g*/
			std::vector<InternalPosition> dispose_list_;
			/*�L�[�|�W�V�����}�b�v<ID,<�폜�\�t���O,�ʒu>>*/
			std::unordered_map<std::string, core::TaskPosition> key_position_map_;

			/*�v���Z�X�̐ݒ�*/
			void SetupProcess_(const std::shared_ptr<Task>& game_process, std::function<bool()>&& remover,const core::TaskPosition& pos);
			/*�v���Z�X�����X�g�ɒǉ�*/
			InternalPosition AddTaskToList_(const std::shared_ptr<Task>& game_process, const core::TaskPosition& pos);
			/*���O��o�^*/
			bool RegisterProcessName_(const std::string& name,Task* id);
			/*ID�}�b�v�Ƀv���Z�X�o�^*/
			bool RegisterToIDMap_(Task* id,const InternalPosition& pos_at_plist);
			/*�ʒu�Ńv���Z�X�폜*/
			bool RemoveTask_(const InternalPosition& pos);
			/*�j�����X�g����������*/
			void ProcessDisposeList_();
			/*�D��x���X�g�ł̒ǉ��ʒu���擾�܂��͍쐬����*/
			core::TaskPriorytyListType::iterator CreateOrGetInsertPositionAtPriorityList_(const core::TaskPosition& pos);

			core::IGameAccessor& game_;
			std::shared_ptr<SceneAccessorForTask> scene_accessor_;
			std::unique_ptr<core::SceneDataForTask> scene_data_;
		};
	}
}