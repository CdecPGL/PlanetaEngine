#pragma once

#include <memory>
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
		enum class SystemTaskSlot;
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

		/*�^�X�N�̃A�b�v�f�[�g*/
		void Update();
		/*�Ǘ�����*/
		bool Process();

		/*�V�X�e���^�X�N�ǉ�(�V�X�e���^�X�N�폜�s�\)*/
		template<class C>
		utility::WeakPointer<C> AddSystemTask(SystemTaskSlot sys_task_slot) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			return std::static_pointer_cast<C>(CreateSystemTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, sys_task_slot));
		}

		/*���O����Q�[���v���Z�X���擾*/
		utility::WeakPointer<Task> GetTask(const std::string& name)override;
		/*�Q�[���v���Z�X�쐻*/
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot)override;
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name)override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		/*�V�X�e���^�X�N�쐻*/
		std::shared_ptr<Task> CreateSystemTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, core::SystemTaskSlot slot);
	};
}