#pragma once

#include <memory>
#include <string>

#include "WeakPointer.hpp"
#include "TaskSlot.hpp"

namespace plnt {
	class Task;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_task_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_task_manager() = 0 {}

		/*! ゲームプロセスを名前から取得する*/
		[[nodiscard]] virtual weak_pointer<Task> get_task(const std::string &name) const = 0;

		/*! タスクを作成*/
		template <class T>
		[[nodiscard]] weak_pointer<T> create_task(task_slot slot) {
			auto task = std::make_shared<T>();
			return register_task(task, slot, false) ? task : nullptr;
		}

		/*名前付きタスクを作成*/
		template <class T>
		[[nodiscard]] weak_pointer<T> create_task(task_slot slot, const std::string &name) {
			auto task = std::make_shared<T>();
			return register_task(task, slot, name, false) ? task : nullptr;
		}

		/*! タスクを作成して実行*/
		template <class T>
		[[nodiscard]] weak_pointer<T> create_and_run_task(task_slot slot) {
			auto task = std::make_shared<T>();
			return register_task(task, slot, true) ? task : nullptr;
		}

		/*名前付きタスクを作成して実行*/
		template <class T>
		[[nodiscard]] weak_pointer<T> create_and_run_task(task_slot slot, const std::string &name) {
			auto task = std::make_shared<T>();
			return register_task(task, slot, name, true) ? task : nullptr;
		}

	private:
		/*ゲームプロセスを登録する*/
		virtual bool register_task(const std::shared_ptr<Task> &task, task_slot slot, bool auto_run) = 0;
		/*ゲームプロセスを登録して名前をつける*/
		virtual bool register_task(const std::shared_ptr<Task> &task, task_slot slot, const std::string &name,
		                           bool auto_run) = 0;
	};
}
