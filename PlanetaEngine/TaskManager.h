#pragma once

#include <memory>
#include "SceneModule.h"
#include "TaskManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		class ScenePublicInterface;
		struct SceneData;
		struct SceneDataForTask;
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
		/*シーンインターフェイスをセット*/
		void SetSceneInterface(core::ScenePublicInterface& spi)override;
		/*シーンデータをセット*/
		void SetSceneData(const core::SceneData& scene_data)override;

		/*タスクのアップデート*/
		void Update();
		/*管理処理*/
		bool Process();

		/*システムタスク追加(システムタスクはIDマップや名前マップに登録されないため、削除不可能)*/
		template<class C>
		utility::WeakPointer<C> AddSystemProcess(SystemTaskSlot sys_task_slot) {
			static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
			return std::static_pointer_cast<C>(CreateSystemTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, sys_task_slot));
		}

		/*名前からゲームプロセスを取得*/
		utility::WeakPointer<Task> GetTask(const std::string& name)override;
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