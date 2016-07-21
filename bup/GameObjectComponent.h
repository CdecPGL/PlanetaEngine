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
	class JSONObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	/*! @brief ゲームオブジェクトコンポーネントの基底クラス
		@attention クラス宣言の先頭にPE_CLONABLE_GAMEOBJECTCOMPONENTマクロ、抽象クラスならPE_COPIABLE_GAMEOBJECTCOMPONENTを記述しなければならない。
		@warning ゲームオブジェクトコンポーネントの定義の際には、このクラスを直接継承してはいけない。代わりにGameObjectStandardComponentを継承すること。
	*/
	class GameObjectComponent : public core::Object, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		/*! @brief ゲームオブジェクトコンポーネントが使用可能か
			
			所有されているゲームオブジェクトが破棄されているときや、正しく初期化されていないときにfalseを返す。
		*/
		bool is_valied()const { return is_valied_; }
		//! 所有されているゲームオブジェクトがシーン内で有効状態か
		bool is_active()const { return is_active_; }

		/*システム関数(GameObjectBaseから呼び出される)*/
		//ファイルの読み込み
		bool Load(const JSONObject& json_obj);
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
		//! コンポーネント取得処理関数
		virtual bool GetOtherComponentProc(const GOComponentGetter& com_getter) { return true; }
		/*! @brief ロード処理関数
			
			所有されているゲームオブジェクトの初期化前に呼び出される。
			引数のGOComponentGetterを用いて所有されているゲームオブジェクトの他のコンポーネントを取得する。<br/>
			デフォルトではfalseを返すので、ファイル定義読み込みに対応させたい場合は必ずオーバーライドする。<br/>
			この関数がfalseを返すコンポーネントを含むゲームオブジェクトをファイル定義から読み込む場合、定義にそのコンポーネントの定義が含まれているとエラーになる。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。

			@note 現在は、所有されているゲームオブジェクトごとの初期化前に呼び出されるが、今後、データのロード時に一度だけ呼び出されるように変更される可能性あり。
			@param データを格納したJSONObject
			@return データ読み込みの成否
		*/
		virtual bool LoadProc(const JSONObject& json_obj) { return false; }
		/*イベント関数*/
		/*! @brief 初期化時イベント関数

			所属するゲームオブジェクトが初期化されたときに呼び出される。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnInitialized() { return true; };
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
		virtual void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data) = 0;
	};
}

/*! @def
	GameObejectComponentをシステムに登録する
	@param(type) 登録したい型
*/
#define PE_REGISTER_GAMEOBJECTCOMPONENT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectComponent)