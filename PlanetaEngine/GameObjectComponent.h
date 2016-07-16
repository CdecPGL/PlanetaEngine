#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"

namespace planeta {
	class SceneAccessorForGameObject;
	class JSONObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	class GameObjectComponent : public core::Object, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }

		/*システム関数(GameObjectBaseから呼び出される)*/
		bool Load(const JSONObject& json_obj);
		bool Initialize();
		bool Activate();
		bool InActivate();
		void Finalize();
	protected:
		//保持されているゲームオブジェクトへの参照
		IGameObjectForComponent& game_object();
	private:
		bool is_valied_ = false;
		bool is_active_ = false;

		util::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*特別設定関数*/
		virtual void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data) = 0;

		/*イベント関数*/
		virtual bool OnInitialized() { return true; }; //所属するゲームオブジェクトが生成されたときに呼び出される(システム関数)
		virtual bool OnActivated() { return true; }; //所属するゲームオブジェクトがシーンに登録されるときに呼び出される(システム関数)
		virtual bool OnInactivated() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		virtual void OnFinalized()noexcept {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
	};
}

//GameObejectComponentをシステムに登録する(型)
#define PE_REGISTER_GAMEOBJECTCOMPONENT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectComponent)