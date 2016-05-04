#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "Delegate.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		struct SceneData;
		struct TaskManagerConnection;
	}
	class IGameObject;
	class Task :
		public core::Object, private utility::NonCopyable<Task> {
	public:
		using GameObjectAccessorType = utility::WeakPointer<IGameObject>;
		Task(core::IGameAccessor& gameaccess) :game_(gameaccess) {}
		virtual ~Task();
		virtual void Update() = 0;
		bool Pause();
		bool Resume();
		void Dispose();
		/*システム関数*/
		bool SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const utility::WeakPointer<core::SceneData>& scene_data);
		/*イベント*/
		/*プロセスが破棄された*/
		utility::Delegate<void> disposed;
		/*ユーティリティ関数*/

	protected:
		core::IGameAccessor& game_accessor() { return game_; }
		//ゲームオブジェクトを作成
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//ゲームオブジェクトを作成して有効化
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//タスクを作成
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot) {
			return RefTaskManagerInterface_().CreateTask(slot);
		}
		//名前付きタスクを作成
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			return RefTaskManagerInterface_().CreateTask(slot, name);
		}
		//型でタスク取得
		template<class T>
		utility::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//名前でタスクを取得
		utility::WeakPointer<Task> GetTaskByName(const std::string& name)const;

	private:
		core::IGameAccessor& game_;
		utility::WeakPointer<core::SceneData> scene_data_;
		std::unique_ptr<core::TaskManagerConnection> manager_connection_;
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
}