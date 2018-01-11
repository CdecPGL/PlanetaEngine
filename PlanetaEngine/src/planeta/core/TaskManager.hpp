#pragma once

#include "planeta/core/ITaskManager.hpp"
#include "SceneModule.hpp"
#include "SystemTaskSlot.hpp"

namespace plnt {
	namespace private_ {
		class TaskManager : public ITaskManager, public SceneModule{
		public:
			virtual ~TaskManager() = 0 {};
			virtual void ExcuteTask() = 0;
			/*�V�X�e���^�X�N�ǉ�(�V�X�e���^�X�N�폜�s�\)*/
			template<class C>
			WeakPointer<C> AddSystemTask(private_::SystemTaskSlot sys_task_slot) {
				static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
				auto task = std::make_shared<C>();
				return std::static_pointer_cast<C>(RegisterSystemTask(task, sys_task_slot));
			}
		private:
			virtual std::shared_ptr<Task> RegisterSystemTask(const std::shared_ptr<Task>& task, private_::SystemTaskSlot slot) = 0;
		};
	}
}
