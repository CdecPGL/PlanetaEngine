#pragma once

#include <memory>
#include <functional>
#include <string>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "TaskSlot.h"

namespace planeta {
	class Task;
	class ITaskManager {
	public:
		virtual ~ITaskManager() = default;
		/*! �Q�[���v���Z�X�𖼑O����擾����*/
		virtual WeakPointer<Task> GetTask(const std::string& name)const = 0;
		/*! �^�X�N���쐬*/
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RegisterTask(task, slot, false) ? task : nullptr;
		}
		/*���O�t���^�X�N���쐬*/
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name, false) ? task : nullptr;
		}
		/*! �^�X�N���쐬���Ď��s*/
		template<class T>
		WeakPointer<T> CreateAndRunTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RegisterTask(task, slot, true) ? task : nullptr;
		}
		/*���O�t���^�X�N���쐬���Ď��s*/
		template<class T>
		WeakPointer<T> CreateAndRunTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name, true) ? task : nullptr;
		}
	private:
		/*�Q�[���v���Z�X��o�^����*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, bool auto_run) = 0;
		/*�Q�[���v���Z�X��o�^���Ė��O������*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name, bool auto_run) = 0;
	};
}