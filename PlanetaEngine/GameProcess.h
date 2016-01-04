#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"
#include "GameProcessPosition.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class SceneAccessorForGameProcess;
		struct SceneDataForGameProcess;
		struct GameProcessRegistrationData;
	}
	namespace game{
		class IGameObjectAccessor;
		class GameProcess :
			public core::Object
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
			utility::WeakPointerDelegate<> disposed;
		protected:
			core::IGameAccessor& game_accessor() { return game_; }
			core::SceneAccessorForGameProcess& scene() { return *scene_accessor_; }
		private:
			GameProcess(const GameProcess&) = delete;
			GameProcess(GameProcess&&) = delete;
			GameProcess& operator=(const GameProcess&) = delete;
			GameProcess& operator=(GameProcess&&) = delete;
			core::IGameAccessor& game_;
			utility::WeakPointer<core::SceneAccessorForGameProcess> scene_accessor_;
			virtual bool OnCreated() { return true; }
			virtual void OnDisposed() {}
			std::function<void()> disposer_;
			std::unique_ptr<core::GameProcessPosition> gameprocess_position_;
		};
	}
}