#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class SceneAccessorForGameProcess;
	}
	namespace game{
		class IGameObjectAccessor;
		class GameProcess :
			public core::Object
		{
		public:
			GameProcess(core::IGameAccessor& gameaccess,int id):game_(gameaccess),id_(id){}
			virtual ~GameProcess() = default;
			virtual void Update() = 0;
			/*システム関数*/
			void SetScene(const utility::WeakPointer<core::SceneAccessorForGameProcess>& scene) { scene_accessor_ = scene; }
			/*イベント*/
			/*プロセスが破棄された*/
			utility::WeakPointerDelegate<> disposed;

			const int id()const { return id_; }
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
			int id_ = 0;
		};
	}
}