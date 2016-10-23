#pragma once

#include <memory>
#include <functional>
#include <string>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "TaskSlot.h"

namespace planeta {
	namespace private_ {
		class IGameAccessor;
	}
	class Task;
	class TaskManagerPublicInterface {
	public:
		virtual ~TaskManagerPublicInterface() = default;
		/*ゲームプロセスを登録する*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, bool auto_run) = 0;
		/*ゲームプロセスを登録して名前をつける*/
		virtual bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name, bool auto_run) = 0;
		/*ゲームプロセスを名前から取得する*/
		virtual WeakPointer<Task> GetTask(const std::string& name)const = 0;
	};
}