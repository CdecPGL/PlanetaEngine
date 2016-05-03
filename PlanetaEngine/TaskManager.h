#pragma once

#include <memory>
#include "SceneModule.h"
#include "TaskManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		struct SceneData;
		enum class SystemTaskSlot;
	}
	class Task;
	class TaskManager final : public core::SceneModule
		, public TaskManagerPublicInterface {
	public:
		TaskManager(core::IGameAccessor& game);
		~TaskManager();
		/*初期化処理*/
		bool Initialize()override { return true; }
		/*終了処理*/
		void Finalize()override;
		/*シーンデータをセット*/
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data)override;

		/*タスクの実行*/
		void ExcuteTask();
		/*管理処理*/
		void Update()override;

		/*システムタスク追加(システムタスク削除不可能)*/
		template<class C>
		utility::WeakPointer<C> AddSystemTask(core::SystemTaskSlot sys_task_slot) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			return std::static_pointer_cast<C>(CreateSystemTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, sys_task_slot));
		}

		/*名前からゲームプロセスを取得*/
		utility::WeakPointer<Task> GetTask(const std::string& name)const override;
		/*ゲームプロセス作製*/
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot)override;
		std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, TaskSlot slot, const std::string& name)override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		/*システムタスク作製*/
		std::shared_ptr<Task> CreateSystemTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, core::SystemTaskSlot slot);
	};
}