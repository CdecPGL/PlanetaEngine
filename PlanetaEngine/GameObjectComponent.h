#pragma once
#include <memory>
#include <functional>
#include"Object.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class SceneAccessorForGameObject;
	}
	namespace game{
		class GameObject;
		class IGameObjectAccessor;
		class GameObjectComponent : public core::Object , public utility::SharedPointerInstance<GameObjectComponent>
		{
			friend class GameObject;
		public:
			GameObjectComponent() :id_(-1){};
			virtual ~GameObjectComponent() = default;
			void SetGameObject(const utility::WeakPointer<IGameObjectAccessor>& game_object, int id) { game_object_ = game_object; id_ = id; }
			void SetSceneAccessor(const utility::WeakPointer<core::SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			

			bool is_valied()const{ return is_valied_; }
			bool is_active()const { return is_active_; }
			IGameObjectAccessor& game_object() { assert(game_object_ != nullptr); return *game_object_; }
			const IGameObjectAccessor& game_object()const { assert(game_object_ != nullptr); return *game_object_; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			core::SceneAccessorForGameObject& scene() { return *scene_accessor_; }
		private:
			GameObjectComponent(const GameObjectComponent&) = delete;
			GameObjectComponent(GameObjectComponent&&) = delete;
			GameObjectComponent& operator=(const GameObjectComponent&) = delete;
			GameObjectComponent& operator=(GameObjectComponent&&) = delete;

//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

			bool is_valied_ = false;
			bool is_active_ = false;
			int id_;

			utility::WeakPointer<IGameObjectAccessor> game_object_;
			utility::WeakPointer<core::SceneAccessorForGameObject> scene_accessor_;

			/*システム関数*/
			virtual bool NoUpdate_()const { return false; } //更新処理を行わないか
			bool Initialize();
			bool Activate();
			void Update() { Update_(); }
			bool InActivate();
			void Finalize();

			virtual bool Initialize_() { return true; }; //所属するゲームオブジェクトが生成されたときに呼び出される(システム関数)
			virtual bool Activated_() { return true; }; //所属するゲームオブジェクトがシーンに登録されるときに呼び出される(システム関数)
			virtual void Update_() { if (NoUpdate_()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //マイフレーム呼び出される(システム関数)
			virtual bool InActivated_() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
			virtual void Finalize_()throw() {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
		};
	}
}