#pragma once

#include <memory>
#include "task_manager.hpp"
#include "weak_pointer.hpp"

namespace plnt {
	class task;

	namespace private_ {
		enum class system_task_slot;

		class standard_task_manager final : public task_manager {
		public:
			standard_task_manager();
			standard_task_manager(const standard_task_manager &) = delete;
			standard_task_manager(standard_task_manager &&) = delete;
			~standard_task_manager() override;
			standard_task_manager &operator=(const standard_task_manager &) = delete;
			standard_task_manager &operator=(standard_task_manager &&) = delete;

			/*初期化処理*/
			bool initialize() override { return true; }
			/*終了処理*/
			void finalize() override;

			/*タスクの実行*/
			void execute_task() override;
			/*管理処理*/
			void update() override;

			/*名前からゲームプロセスを取得*/
			[[nodiscard]] weak_pointer<task> get_task(const std::string &name) const override;
			/*ゲームプロセス作製*/
			bool register_task(const std::shared_ptr<task> &task, task_slot slot, bool auto_run) override;
			bool register_task(const std::shared_ptr<task> &task, task_slot slot, const std::string &name,
			                   bool auto_run) override;

		private:
			class impl;
			std::unique_ptr<impl> impl_;

			/*システムタスク作製*/
			std::shared_ptr<task>
			register_system_task(const std::shared_ptr<task> &task, private_::system_task_slot slot) override;

			void debug_information_add_handle(i_debug_information_adder &di_adder) override;
		};
	}
}
