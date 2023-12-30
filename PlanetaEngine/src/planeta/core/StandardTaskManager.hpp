#pragma once

#include <memory>
#include "TaskManager.hpp"
#include "WeakPointer.hpp"

namespace plnt {
	class Task;

	namespace private_ {
		enum class SystemTaskSlot;

		class StandardTaskManager final : public TaskManager {
		public:
			StandardTaskManager();
			~StandardTaskManager();
			/*初期化処理*/
			bool initialize() override { return true; }
			/*終了処理*/
			void finalize() override;

			/*タスクの実行*/
			void ExcuteTask() override;
			/*管理処理*/
			void update() override;

			/*名前からゲームプロセスを取得*/
			WeakPointer<Task> get_task(const std::string &name) const override;
			/*ゲームプロセス作製*/
			bool register_task(const std::shared_ptr<Task> &task, TaskSlot slot, bool auto_run) override;
			bool register_task(const std::shared_ptr<Task> &task, TaskSlot slot, const std::string &name,
			                  bool auto_run) override;

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;

			/*システムタスク作製*/
			std::shared_ptr<Task>
			RegisterSystemTask(const std::shared_ptr<Task> &task, private_::SystemTaskSlot slot) override;

			void debug_information_add_handle(i_debug_information_adder &di_adder) override;
		};
	}
}
