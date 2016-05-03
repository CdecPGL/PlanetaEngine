#pragma once
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace core {
		class Scene;
	}
	class IGameObject;
	class SceneSetUpProxy {
	public:
		SceneSetUpProxy(core::Scene& scene);
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
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		core::Scene& scene_;
	};
}