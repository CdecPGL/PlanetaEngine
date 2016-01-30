#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	namespace game {
		class IGameObjectAccessor;
		class GameObject;
	}
	namespace core{
		struct SceneDataForGameObject;
		struct GameObjectComponentRegistrationData;
	}
	class GameObjectComponent : public core::Object, public utility::SharedPointerInstance<GameObjectComponent>, private utility::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() :id_(-1) {};
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentRegistrationData& resistration_data, const core::SceneDataForGameObject& special_setup_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }
		game::IGameObjectAccessor& game_object() { assert(game_object_ != nullptr); return *game_object_; }
		const game::IGameObjectAccessor& game_object()const { assert(game_object_ != nullptr); return *game_object_; }

		/*システム関数*/
		virtual bool is_no_update()const { return false; } //更新処理を行わないか
		bool Initialize();
		bool Activate();
		void Update() { OnUpdated(); }
		bool InActivate();
		void Finalize();
	protected:
		using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
		SceneAccessorForGameObject& scene() { return *scene_accessor_; }
	private:
		//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
		//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
		//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
		//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

		bool is_valied_ = false;
		bool is_active_ = false;
		int id_;

		utility::WeakPointer<game::IGameObjectAccessor> game_object_;
		utility::WeakPointer<SceneAccessorForGameObject> scene_accessor_;

		/*特別設定関数*/
		virtual bool SpecialSetUp(const core::SceneDataForGameObject& setup_data) = 0;

		/*イベント関数*/
		virtual bool OnInitialized() { return true; }; //所属するゲームオブジェクトが生成されたときに呼び出される(システム関数)
		virtual bool OnActivated() { return true; }; //所属するゲームオブジェクトがシーンに登録されるときに呼び出される(システム関数)
		virtual void OnUpdated() { if (is_no_update()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //マイフレーム呼び出される(システム関数)
		virtual bool OnInactivated() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		virtual void OnFinalized()noexcept {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
	};
}