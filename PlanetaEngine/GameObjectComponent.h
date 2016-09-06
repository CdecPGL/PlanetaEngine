#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"
#include "GOComponentGetter.h"

namespace planeta {
	class SceneAccessorForGameObject;
	namespace private_{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	/*! @brief ゲームオブジェクトコンポーネントの基底クラス
		@attention クラス宣言の先頭にPE_CLONABLE_GAMEOBJECTCOMPONENTマクロ、抽象クラスならPE_COPIABLE_GAMEOBJECTCOMPONENTを記述しなければならない。
		@warning ゲームオブジェクトコンポーネントの定義の際には、このクラスを直接継承してはいけない。代わりにGameObjectStandardComponentを継承すること。
	*/
	class GameObjectComponent : public Object, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
	public:
		using Super = Object;
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SetSceneAndHolderGOData(const private_::GameObjectComponentSetUpData& resistration_data);

		/*! @brief ゲームオブジェクトコンポーネントが使用可能か
			
			所有されているゲームオブジェクトが破棄されているときや、正しく初期化されていないときにfalseを返す。
		*/
		bool is_valied()const { return is_valied_; }
		//! 所有されているゲームオブジェクトがシーン内で有効状態か
		bool is_active()const { return is_active_; }

		/*システム関数(GameObjectBaseから呼び出される)*/
		//ほかコンポーネント取得と初期化
		bool Initialize(const GOComponentGetter& com_getter);
		bool Activate();
		bool InActivate();
		void Finalize();
	protected:
		/*! @brief 所有されているゲームオブジェクトへの参照
			
			通常のIGameObjectではなく、ゲームオブジェクトコンポーネント用に関数が追加されたものを返す。
		*/
		IGameObjectForComponent& game_object();
		//オーバーライド可能関数
		/*イベント関数*/
		/*! @brief 初期化時イベント関数

			所属するゲームオブジェクトが初期化されたときに呼び出される。ほかのコンポーネントの取得もここで行う。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnInitialized(const GOComponentGetter&) { return true; };
		/*! @brief 有効化時イベント関数

			所属するゲームオブジェクトが有効化されたときに呼び出される。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnActivated() { return true; };
		/*! @brief 無効化時イベント関数

		所属するゲームオブジェクトが無効化されたときに呼び出される。<br/>
		末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnInactivated() { return true; } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		/*! @brief 破棄時イベント関数

			所属するゲームオブジェクトが破棄される直前に呼び出される。<br/>
			末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void OnFinalized()noexcept {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
	private:
		bool is_valied_ = false;
		bool is_active_ = false;

		util::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*特別設定関数*/
		virtual void SetSceneData(const util::WeakPointer<private_::SceneData>& scene_data) = 0;
	};
	PE_REFLECTABLE_CLASS(GameObjectComponent);
}

#define PE_GAMEOBJECTCOMPONENT_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
