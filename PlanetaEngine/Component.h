#pragma once
#include <memory>
#include <functional>
#include"Object.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ISceneAccessForGameObject;
	}
	namespace game{
		class GameObject;
		class IGameObjectAccessor;
		class Component : public core::Object , public utility::SharedPointerInstance<Component>
		{
			friend class GameObject;
		public:
			Component() :_id(-1){};
			virtual ~Component() = default;
			void SetGameObject(const utility::WeakPointer<IGameObjectAccessor>& game_object, int id) { _game_object = game_object; _id = id; }
			void SetSceneAccessor(const utility::WeakPointer<core::ISceneAccessForGameObject>& scene) { _scene = scene; }

			//GameObjectの複製時に呼ばれる(システム関数)
//			virtual std::shared_ptr<Component> Clone(GameObject& go,std::vector<std::shared_ptr<Component>*>& need_update_pointer_list); //別のゲームオブジェクトのコンポーネントとして自身の複製を作成(その際第二引数に、更新が必要なコンポーネントリストが渡されるので、必要なら登録する)
			

			bool is_valied()const{ return _is_valied; }
			utility::WeakPointer<IGameObjectAccessor> game_object()const { return _game_object; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			utility::WeakPointer<core::ISceneAccessForGameObject> scene() { return _scene; }
		private:
			Component(const Component&) = delete;
			Component(Component&&) = delete;
			Component& operator=(const Component&) = delete;
			Component& operator=(Component&&) = delete;

//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

//			bool _update_component_pointer(std::shared_ptr<Component>*); //コンポーネントを示すポインタをコンポーネントIDをもとに、現在のゲームオブジェクトから取得する

			bool _is_valied;
			int _id;

			utility::WeakPointer<IGameObjectAccessor> _game_object;
			utility::WeakPointer<core::ISceneAccessForGameObject> _scene;

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