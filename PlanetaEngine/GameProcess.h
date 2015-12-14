#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"

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
			GameProcess(core::IGameAccessor& gameaccess):game_(gameaccess){}
			virtual ~GameProcess() = default;
			virtual void Update() = 0;
			void Dispose();
			/*システム関数*/
			bool SystemSetUpAndInitialize(const core::GameProcessRegistrationData& resistration_data, const core::SceneDataForGameProcess& special_setup_data);
			/*イベント*/
			/*プロセスが破棄された*/
			utility::WeakPointerDelegate<> disposed;
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
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
		};
	}
}