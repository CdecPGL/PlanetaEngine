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
		/*�V�X�e���֐�*/
		bool SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const utility::WeakPointer<core::SceneData>& scene_data);
		/*�C�x���g*/
		/*�v���Z�X���j�����ꂽ*/
		utility::Delegate<void> disposed;
		/*���[�e�B���e�B�֐�*/

	protected:
		core::IGameAccessor& game_accessor() { return game_; }
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
		core::IGameAccessor& game_;
		utility::WeakPointer<core::SceneData> scene_data_;
		std::unique_ptr<core::TaskManagerConnection> manager_connection_;
		TaskManagerPublicInterface& RefTaskManagerInterface_();
		const TaskManagerPublicInterface& RefTaskManagerInterface_()const;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
}