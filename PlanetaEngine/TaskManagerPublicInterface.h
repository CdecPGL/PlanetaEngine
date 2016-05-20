#pragma once

#include <memory>
#include <functional>
#include <string>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "TaskSlot.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	class Task;
	class TaskManagerPublicInterface {
	public:
		virtual ~TaskManagerPublicInterface() = default;
		/*�Q�[���v���Z�X���쐬����*/
		template<class C>
		utility::WeakPointer<C> CreateTask(TaskSlot slot) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			return std::static_pointer_cast<C>(CreateTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, slot));
		}
		/*�Q�[���v���Z�X���쐬���Ė��O������*/
		template<class C>
		utility::WeakPointer<C> CreateTask(TaskSlot slot, const core::TaskPosition& pos) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			return std::static_pointer_cast<C>(CreateTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, slot, const std::string& name));
		}
		/*�Q�[���v���Z�X�𖼑O����擾����*/
		virtual utility::WeakPointer<Task> GetProcess(const std::string& name) = 0;
	private:
		virtual std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot) = 0;
		virtual std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name) = 0;
	};
}