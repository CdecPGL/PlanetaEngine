#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "Delegate.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	class ISceneManagerAccessor;
	namespace core {
		struct SceneData;
		class TaskManagerConnection;
	}
	class IGameObject;
	class Task :
		public core::Object, private utility::NonCopyable<Task> {
	public:
		using GameObjectAccessorType = utility::WeakPointer<IGameObject>;
		Task();
		virtual ~Task();
		virtual void Update() = 0;
		bool Pause();
		bool Resume();
		void Dispose();
		/*�V�X�e���֐�*/
		bool SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const utility::WeakPointer<core::SceneData>& scene_data);
		/*�C�x���g*/
		/*�v���Z�X���j�����ꂽ*/
		utility::Delegate<void> disposed;
		/*���[�e�B���e�B�֐�*/

	protected:
		//�Q�[���I�u�W�F�N�g���쐬
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//�^�X�N���쐬
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot) ? task : nullptr;
		}
		//���O�t���^�X�N���쐬
		template<class T>
		utility::WeakPointer<T> CreateTask(TaskSlot slot, const std::string& name) {
			auto task = std::make_shared<T>();
			return RefTaskManagerInterface_().RegisterTask(task, slot, name) ? task : nullptr;
		}
		//�^�Ń^�X�N�擾
		template<class T>
		utility::WeakPointer<T> GetTaskByType()const {
			return RefTaskManagerInterface_().GetTask<T>();
		}
		//���O�Ń^�X�N���擾
		utility::WeakPointer<Task> GetTaskByName(const std::string& name)const;
		//�V�[���}�l�[�W���ւ̃A�N�Z�X
		ISceneManagerAccessor& scene_manager();
	private:
		utility::WeakPointer<core::SceneData> scene_data_;
		std::unique_ptr<core::TaskManagerConnection> manager_connection_;
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
}

//Task���V�X�e���ɓo�^����(�^)
#define PE_REGISTER_TASK(type)\
PE_REGISTER_OBJECT(type,planeta_engine::Task)