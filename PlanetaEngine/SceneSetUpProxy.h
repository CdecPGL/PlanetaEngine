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
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//�^�X�N���쐬
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot) {
			return RefTaskManagerInterface_().CreateTask(slot);
		}
		//���O�t���^�X�N���쐬
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			return RefTaskManagerInterface_().CreateTask(slot, name);
		}
		//�^�Ń^�X�N�擾
		template<class T>
		utility::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//���O�Ń^�X�N���擾
		utility::WeakPointer<Task> GetTaskByName(const std::string& name)const;
	private:
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		core::Scene& scene_;
	};
}