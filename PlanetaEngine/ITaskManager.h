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
		/*! ゲームプロセスを名前から取得する*/
		virtual WeakPointer<Task> GetTask(const std::string& name)const = 0;
		/*! タスクを作成*/
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RegisterTask(task, slot, false) ? task : nullptr;
		}
		/*名前付きタスクを作成*/
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name, false) ? task : nullptr;
		}
		/*! タスクを作成して実行*/
		template<class T>
		WeakPointer<T> CreateAndRunTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RegisterTask(task, slot, true) ? task : nullptr;
		}
		/*名前付きタスクを作成して実行*/
		template<class T>
		WeakPointer<T> CreateAndRunTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name, true) ? task : nullptr;
		}
	private:
		/*ゲームプロセスを登録する*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, bool auto_run) = 0;
		/*ゲームプロセスを登録して名前をつける*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name, bool auto_run) = 0;
	};
}