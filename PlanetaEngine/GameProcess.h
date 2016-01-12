#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"
#include "GameProcessPosition.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class SceneAccessorForGameProcess;
	namespace core{
		class IGameAccessor;
		struct SceneDataForGameProcess;
		struct GameProcessRegistrationData;
	}
	namespace game{
		class IGameObjectAccessor;
		class GameProcess :
			public core::Object , private utility::NonCopyable<GameProcess>
		{
		public:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			GameProcess(core::IGameAccessor& gameaccess):game_(gameaccess){}
			virtual ~GameProcess();
			virtual void Update() = 0;
			void Dispose();
			/*�V�X�e���֐�*/
			bool SystemSetUpAndInitialize(const core::GameProcessRegistrationData& resistration_data, const core::SceneDataForGameProcess& special_setup_data);
			/*�Q�[���v���Z�X���X�g�ł̈ʒu*/
			const core::GameProcessPosition& game_process_position()const;
			/*�C�x���g*/
			/*�v���Z�X���j�����ꂽ*/
			utility::WeakPointerDelegate<void> disposed;
			/*���[�e�B���e�B�֐�*/
			
		protected:
			core::IGameAccessor& game_accessor() { return game_; }
			SceneAccessorForGameProcess& scene() { return *scene_accessor_; }
		private:
			core::IGameAccessor& game_;
			utility::WeakPointer<SceneAccessorForGameProcess> scene_accessor_;
			virtual bool OnCreated() { return true; }
			virtual void OnDisposed() {}
			std::function<void()> disposer_;
			std::unique_ptr<core::GameProcessPosition> gameprocess_position_;
		};
	}
}