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
		class Component : public core::Object , public utility::SharedPointerInstance<Component>
		{
			friend class GameObject;
		public:
			Component() :id_(-1){};
			virtual ~Component() = default;
			void SetGameObject(const utility::WeakPointer<IGameObjectAccessor>& game_object, int id) { game_object_ = game_object; id_ = id; }
			void SetSceneAccessor(const utility::WeakPointer<core::SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			

			bool is_valied()const{ return is_valied_; }
			IGameObjectAccessor& game_object() { return *game_object_; }
			const IGameObjectAccessor& game_object()const { return *game_object_; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			core::SceneAccessorForGameObject& scene() { return *scene_accessor_; }
		private:
			Component(const Component&) = delete;
			Component(Component&&) = delete;
			Component& operator=(const Component&) = delete;
			Component& operator=(Component&&) = delete;

//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

			bool is_valied_;
			int id_;

			utility::WeakPointer<IGameObjectAccessor> game_object_;
			utility::WeakPointer<core::SceneAccessorForGameObject> scene_accessor_;

			/*システム関数*/
			virtual bool NoUpdate_()const { return false; } //更新処理を行わないか

			virtual bool Initialize_() { return true; }; //所属するゲームオブジェクトが生成されたときに呼び出される(システム関数)
			virtual bool Activated_() { return true; }; //所属するゲームオブジェクトがシーンに登録されるときに呼び出される(システム関数)
			virtual void Update_() { if (NoUpdate_()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //マイフレーム呼び出される(システム関数)
			virtual bool InActivated_() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
			virtual void Finalize_()throw() {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
		};
	}
}