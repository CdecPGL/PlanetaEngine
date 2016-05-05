#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	class GameObjectComponent : public core::Object, public utility::SharedPointerInstance<GameObjectComponent>, private utility::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }

		/*システム関数*/
		virtual bool is_no_update()const { return false; } //更新処理を行わないか
		bool Initialize();
		bool Activate();
		void Update() { OnUpdated(); }
		bool InActivate();
		void Finalize();
	protected:
		//保持されているゲームオブジェクトへの参照
		IGameObjectForComponent& game_object();
	private:
		bool is_valied_ = false;
		bool is_active_ = false;

		utility::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*特別設定関数*/
		virtual void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) = 0;

		/*イベント関数*/
		virtual bool OnInitialized() { return true; }; //所属するゲームオブジェクトが生成されたときに呼び出される(システム関数)
		virtual bool OnActivated() { return true; }; //所属するゲームオブジェクトがシーンに登録されるときに呼び出される(システム関数)
		virtual void OnUpdated() { if (is_no_update()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //マイフレーム呼び出される(システム関数)
		virtual bool OnInactivated() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		virtual void OnFinalized()noexcept {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
	};
}