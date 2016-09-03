#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "Delegate.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"

namespace planeta {
	class SceneAccessorForTask;
	class ISceneManagerAccessor;
	namespace core {
		struct SceneData;
		class TaskManagerConnection;
	}
	class IGameObject;
	class Task :
		public core::Object, private util::NonCopyable<Task> {
	public:
		using Super = core::Object;
		using GameObjectAccessorType = util::WeakPointer<IGameObject>;
		Task();
		virtual ~Task();
		virtual void Update() = 0;
		bool Pause();
		bool Resume();
		void Dispose();
		/*システム関数*/
		bool SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const util::WeakPointer<core::SceneData>& scene_data);
		/*イベント*/
		/*プロセスが破棄された*/
		util::Delegate<void> disposed;
		/*ユーティリティ関数*/

	protected:
		//ゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& gameobject_type_id, const std::string& resource_id);
		//ゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& gameobject_type_id, const std::string& resource_id);
		//定義ファイルの読み込みを行わないゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& gameobject_type_id);
		//定義ファイルの読み込みを行わないゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateDefaultGameObject(const std::string& gameobject_type_id);
		//タスクを作成
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot) ? task : nullptr;
		}
		//名前付きタスクを作成
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name) ? task : nullptr;
		}
		//型でタスク取得
		template<class T>
		util::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//名前でタスクを取得
		util::WeakPointer<Task> GetTaskByName(const std::string& name)const;
		//シーンマネージャへのアクセス
		ISceneManagerAccessor& scene_manager();
	private:
		util::WeakPointer<core::SceneData> scene_data_;
		std::unique_ptr<core::TaskManagerConnection> manager_connection_;
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
	PE_REFLECTABLE_CLASS(Task);
}

#define PE_TASK_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
