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
		//�Q�[���I�u�W�F�N�g���쐬
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//�^�X�N���쐬
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot) ? task : nullptr;
		}
		//���O�t���^�X�N���쐬
		template<class T>
		util::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			return RefTaskManagerInterface_().RegisterTask<T>(slot, name);
		}
		//�^�Ń^�X�N�擾
		template<class T>
		util::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//���O�Ń^�X�N���擾
		util::WeakPointer<Task> GetTaskByName(const std::string& name)const;
	private:
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		core::Scene& scene_;
	};
}