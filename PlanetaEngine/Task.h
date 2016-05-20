#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class IGameAccessor;
		struct SceneDataForTask;
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
		bool SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const core::SceneDataForTask& special_setup_data);
		/*�C�x���g*/
		/*�v���Z�X���j�����ꂽ*/
		utility::WeakPointerDelegate<void> disposed;
		/*���[�e�B���e�B�֐�*/

	protected:
		core::IGameAccessor& game_accessor() { return game_; }
		SceneAccessorForTask& scene();
	private:
		core::IGameAccessor& game_;
		std::unique_ptr<core::TaskManagerConnection> manager_connection_;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
}