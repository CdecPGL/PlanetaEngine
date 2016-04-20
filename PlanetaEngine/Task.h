#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"
#include "TaskPosition.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class SceneAccessorForTask;
	namespace core{
		class IGameAccessor;
		struct SceneDataForTask;
		struct TaskRegistrationData;
	}
	namespace game{
		class IGameObjectAccessor;
		class Task :
			public core::Object , private utility::NonCopyable<Task>
		{
		public:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			Task(core::IGameAccessor& gameaccess):game_(gameaccess){}
			virtual ~Task();
			virtual void Update() = 0;
			void Dispose();
			/*�V�X�e���֐�*/
			bool SystemSetUpAndInitialize(const core::TaskRegistrationData& resistration_data, const core::SceneDataForTask& special_setup_data);
			/*�Q�[���v���Z�X���X�g�ł̈ʒu*/
			const core::TaskPosition& game_process_position()const;
			/*�C�x���g*/
			/*�v���Z�X���j�����ꂽ*/
			utility::WeakPointerDelegate<void> disposed;
			/*���[�e�B���e�B�֐�*/
			
		protected:
			core::IGameAccessor& game_accessor() { return game_; }
			SceneAccessorForTask& scene() { return *scene_accessor_; }
		private:
			core::IGameAccessor& game_;
			utility::WeakPointer<SceneAccessorForTask> scene_accessor_;
			virtual bool OnCreated() { return true; }
			virtual void OnDisposed() {}
			std::function<void()> disposer_;
			std::unique_ptr<core::TaskPosition> gameprocess_position_;
		};
	}
}