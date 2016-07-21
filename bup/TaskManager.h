#pragma once

#include <memory>
#include "SceneModule.h"
#include "TaskManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta {
	class SceneAccessorForTask;
	namespace core {
		struct SceneData;
		enum class SystemTaskSlot;
	}
	class Task;
	class TaskManager final : public core::SceneModule
		, public TaskManagerPublicInterface {
	public:
		TaskManager();
		~TaskManager();
		/*初期化処理*/
		bool Initialize()override { return true; }
		/*終了処理*/
		void Finalize()override;
		/*シーンデータをセット*/
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data)override;

		/*タスクの実行*/
		void ExcuteTask();
		/*管理処理*/
		void Update()override;

		/*システムタスク追加(システムタスク削除不可能)*/
		template<class C>
		util::WeakPointer<C> AddSystemTask(core::SystemTaskSlot sys_task_slot) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			auto task = std::make_shared<C>();
			return std::static_pointer_cast<C>(RegisterSystemTask(task, sys_task_slot));
		}

		/*名前からゲームプロセスを取得*/
		util::WeakPointer<Task> GetTask(const std::string& name)const override;
		/*ゲームプロセス作製*/
		bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot)override;
		bool RegisterTask(const std::shared_ptr<Task>& task, TaskSlot slot, const std::string& name)override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		/*システムタスク作製*/
		std::shared_ptr<Task> RegisterSystemTask(const std::shared_ptr<Task>& task, core::SystemTaskSlot slot);
	};
}