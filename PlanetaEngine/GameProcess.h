#pragma once
#include "Object.h"
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class ISceneAccessForGameProcess;
	}
	namespace game{
		class IGameObjectAccessor;
		class GameProcess :
			public core::Object
		{
		public:
			GameProcess(core::IGameAccessor& gameaccess,int id):_game(gameaccess),id_(id){}
			virtual ~GameProcess() = default;
			virtual void Update() = 0;
			/*システム関数*/
			void SetScene(const utility::WeakPointer<core::ISceneAccessForGameProcess>& scene) { _scene = scene; }
			/*イベント*/
			/*プロセスが破棄された*/
			utility::WeakPointerDelegate<> disposed;

			const int id()const { return id_; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			core::IGameAccessor& game_accessor() { return _game; }
			utility::WeakPointer<core::ISceneAccessForGameProcess> scene() { return _scene; }
		private:
			GameProcess(const GameProcess&) = delete;
			GameProcess(GameProcess&&) = delete;
			GameProcess& operator=(const GameProcess&) = delete;
			GameProcess& operator=(GameProcess&&) = delete;
			core::IGameAccessor& _game;
			utility::WeakPointer<core::ISceneAccessForGameProcess> _scene;
			int id_ = 0;
		};
	}
}