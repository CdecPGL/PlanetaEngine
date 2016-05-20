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
		/*�Q�[���v���Z�X��o�^����*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot) = 0;
		/*�Q�[���v���Z�X��o�^���Ė��O������*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name) = 0;
		/*�Q�[���v���Z�X�𖼑O����擾����*/
		virtual utility::WeakPointer<Task> GetTask(const std::string& name)const = 0;
	};
}