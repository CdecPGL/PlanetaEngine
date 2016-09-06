#pragma once

#include "Object.h"
#include "GameObjectComponentHolder.h"
#include "IGameObjectForComponent.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"
#include "GameObjectManagerConnection.h"

namespace planeta {
	class JSONObject;
	namespace private_ {
		struct SceneData;
	}
	class GameObjectComponent;
	class GOComponentAdder;
	class GOComponentGetter;
	/*! @brief GameObjectの具体的な実装を行うクラス。直接用いることはない。
	*/
	class GameObjectBase :public Object, public IGameObjectForComponent, private util::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
	public:
		using Super = Object;
		GameObjectBase();
		~GameObjectBase();

		//インターフェイスのオーバーライド
		//有効化する
		bool Activate()override final;
		//無効化する
		bool Inactivate()override final;
		//破棄する
		void Dispose()override final;
		//自分のstd::shared_ptrを取得する。
		std::shared_ptr<IGameObject> GetSharedPointer() override final;
		//有効化イベントハンドラ登録
		util::DelegateConnection AddActivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;
		//無効化イベントハンドラ登録
		util::DelegateConnection AddInactivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;
		//破棄イベントハンドラ登録
		util::DelegateConnection AddDisposedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;

		//システム用関数(Managerから呼び出される｡GameObjectクラスで隠ぺいする)
		//インスタンス化時の処理
		bool ProcessInstantiation();
		//クローン時の処理
		bool ProcessClonation(const std::shared_ptr<GameObjectBase>& dst);
		//ロード時の処理
		bool ProcessLoading(const JSONObject& json_object);
		//初期化時の処理
		bool ProcessInitialization();
		//有効化時の処理
		bool ProcessActivation();
		//無効化時の処理
		bool ProcessInactivation();
		//破棄時の処理
		bool ProcessDisposal();
		//マネージャコネクションをセット
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		//シーンデータをセット
		void SetSceneData(const util::WeakPointer<private_::SceneData>& scene_data);
		//シーンデータとゲームオブジェクトのデータをコンポーネントにセット
		void SetSceneAndGODataToCOmponents();
	protected:
		//オーバーライド必須関数
		/*! @brief コンポーネントの登録を行う
		
			シーン内で、GameObjectTypeID、ResourceIDの指定が初めての組み合わせでゲームオブジェクトを生成した場合に一度だけ呼び出される。<br/>
			引数のGOComponentAdderを利用してコンポーネントの追加を行う。<br/>
			関数の先頭で、親クラスの同関数を呼び出す必要がある。
		*/
		virtual void AddComponentsProc(GOComponentAdder& com_adder);
		//イベント関数
		/*! @brief 初期化イベント関数

			ゲームオブジェクトが生成されたときに呼び出される。<br/>
			タスクの作成など、ゲームオブジェクトの初期化処理と必要なコンポーネントへの参照の取得を行う。戻り値は初期化の成否を示す。<br/>
			関数の先頭で、親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnInitialized(const GOComponentGetter& com_getter);
		/*! @brief 有効化イベント関数
		
			ゲームオブジェクトが有効化されるたびに呼び出される。<br/>
			ゲームオブジェクトの有効化処理を行う。戻り値は有効化の成否を示す。<br/>
			関数の先頭で、親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnActivated();
		/*! @brief 無効化イベント関数

			ゲームオブジェクトが無効化されるたびに呼び出される。<br/>
			ゲームオブジェクトの無効化処理を行う。戻り値は無効化の成否を示す。<br/>
			関数の末尾で、親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnInactivated();
		/*! @brief 破棄時イベント関数
		
			ゲームオブジェクトが破棄される直前に呼び出される。<br/>
			ゲームオブジェクトの終了処理を行う。戻り値は終了処理の成否を示す。<br/>
			関数の末尾で、親クラスの同関数を呼び出す必要がある。
		*/
		virtual bool OnDisposed();
	private:
		//インターフェイスのオーバーライド
		//ゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& id, const std::string& resource_id)override final;
		//ゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id, const std::string& resource_id)override final;
		//デフォルトゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& id)override final;
		//デフォルトゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateDefaultGameObject(const std::string& id)override final;
		//コンポーネントを型で取得
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		//コンポーネントを型ですべて取得
		//std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		TaskManagerPublicInterface& RefTaskManagerInterface_()override final;
		void SetUpAttachedTask_(TGameObjectOperation& task)override final;

		//マネージャコネクション
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//シーンアクセサ
		util::WeakPointer<private_::SceneData> scene_data_;
		//コンポーネントホルダー
		GameObjectComponentHolder component_holder_;

		//コンポーネント関連の処理

		void SetSceneAndGODataToComponent_(GameObjectComponent& com);
		
		//イベントデリゲート
		util::Delegate<void> activated_event_delegate_;
		util::Delegate<void> inactivated_event_delegate_;
		util::Delegate<void> disposed_event_delegate_;
	};
	PE_REFLECTABLE_CLASS(GameObjectBase);
}
