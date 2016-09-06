#pragma once
#include "TaskManagerPublicInterface.h"

namespace planeta {
	namespace private_ {
		class Scene;
	}
	class IGameObject;
	class SceneSetUpProxy {
	public:
		SceneSetUpProxy(private_::Scene& scene);
		//ゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//ゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//タスクを作成
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot) ? task : nullptr;
		}
		//名前付きタスクを作成
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			return RefTaskManagerInterface_().RegisterTask<T>(slot, name);
		}
		//型でタスク取得
		template<class T>
		util::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//名前でタスクを取得
		util::WeakPointer<Task> GetTaskByName(const std::string& name)const;
	private:
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		private_::Scene& scene_;
	};
}