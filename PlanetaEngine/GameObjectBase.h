#pragma once

#include "Object.h"
#include "GameObjectComponentHolder.h"
#include "IGameObjectForComponent.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"
#include "GameObjectManagerConnection.h"

namespace planeta_engine {
	namespace core {
		struct SceneData;
	}
	class GameObjectComponent;
	class GameObjectBase :public core::Object, public IGameObjectForComponent, private util::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
	public:
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
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data);
	protected:
		//コンポーネントを作成、追加する。
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto com = component_holder_.CreateAndAddComponent<ComT>();
			SetUpGameComponent(*com);
			return com;
		}
		//イベント関数
		//初期化イベント
		virtual bool OnInitialized();
		//有効化イベント
		virtual bool OnActivated();
		//無効化イベント
		virtual bool OnInactivated();
		//破棄時イベント
		virtual bool OnDisposed();
	private:
		//インターフェイスのオーバーライド
		//ゲームオブジェクトを作成
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& id)override final;
		//ゲームオブジェクトを作成して有効化
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id)override final;
		//コンポーネントを型で取得
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		//コンポーネントを型ですべて取得
		std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		TaskManagerPublicInterface& RefTaskManagerInterface_()override final;
		void SetUpAttachedTask_(TGameObjectOperation& task)override final;

		//マネージャコネクション
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//シーンアクセサ
		util::WeakPointer<core::SceneData> scene_data_;
		//コンポーネントホルダー
		GameObjectComponentHolder component_holder_;

		//コンポーネント関連の処理

		void SetUpGameComponent(GameObjectComponent& com);
		
		//イベントデリゲート
		util::Delegate<void> activated_event_delegate_;
		util::Delegate<void> inactivated_event_delegate_;
		util::Delegate<void> disposed_event_delegate_;
	};
}
