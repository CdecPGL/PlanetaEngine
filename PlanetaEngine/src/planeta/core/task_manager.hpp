#pragma once

#include "i_task_manager.hpp"
#include "scene_module.hpp"
#include "SystemTaskSlot.hpp"

namespace plnt::private_ {
	class task_manager : public i_task_manager, public scene_module {
	public:
		task_manager() = default;
		task_manager(const task_manager &) = delete;
		task_manager(task_manager &&) = delete;
		~task_manager() override = default;
		task_manager &operator=(const task_manager &) = delete;
		task_manager &operator=(task_manager &&) = delete;

		virtual void execute_task() = 0;
		/*システムタスク追加(システムタスク削除不可能)*/
		template <class C>
		weak_pointer<C> add_system_task(system_task_slot sys_task_slot) {
			static_assert(std::is_base_of_v<task, C> == true, "C is not derived Task.");
			auto task = std::make_shared<C>();
			return std::static_pointer_cast<C>(register_system_task(task, sys_task_slot));
		}

	private:
		virtual std::shared_ptr<task> register_system_task(const std::shared_ptr<task> &task,
		                                                   system_task_slot slot) = 0;
	};
}
