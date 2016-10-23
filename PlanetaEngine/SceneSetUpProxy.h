#pragma once
#include "TaskManagerPublicInterface.h"
#include "GameObjectManagerPublicInterface.h"

namespace planeta {
	namespace private_ {
		class Scene;
	}
	class IGameObject;
	class SceneSetUpProxy {
	public:
		SceneSetUpProxy(private_::Scene& scene);
		//! ゲームオブジェクトマネージャへのアクセス
		GameObjectManagerPublicInterface& game_object_manager();
		//タスクを作成
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, true) ? task : nullptr;
		}
		//名前付きタスクを作成
		template<class T>
		WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			return RefTaskManagerInterface_().RegisterTask<T>(slot, name, true);
		}
		//型でタスク取得
		template<class T>
		WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//名前でタスクを取得
		WeakPointer<Task> GetTaskByName(const std::string& name)const;
	private:
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		private_::Scene& scene_;
	};
}