#pragma once

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
	class GameObjectBase : public IGameObjectForComponent, private utility::NonCopyable<GameObjectBase> {
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
		std::shared_ptr<IGameObject> GetSharedPointer()const override final;
		//有効化イベントハンドラ登録
		utility::DelegateConnection AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//無効化イベントハンドラ登録
		utility::DelegateConnection AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//破棄イベントハンドラ登録
		utility::DelegateConnection AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;

		//システム用関数(Managerから呼び出される｡GameObjectクラスで隠ぺいする)
		//有効化時の処理
		bool ProcessActivation();
		//無効化時の処理
		bool ProcessInactivation();
		//破棄時の処理
		bool ProcessDisposal();
		//マネージャコネクションをセット
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		//シーンデータをセット
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data);


		//静的関数
		//GameObjectの作成関数。自分のスマートポインタを保持するためにこの関数を介する必要がある。
		template<class T>
		static std::shared_ptr<GameObjectBase> Create() {
			static_assert(std::is_base_of<GameObjectBase, T>::value == true, "T must derive GameObjectBase");
			auto ptr = std::make_shared<T>();
			ptr->this_weak_ptr_ = ptr;
			return std::move(ptr);
		}
	protected:
		//コンポーネントを作成、追加する。
		template<class ComT>
		utility::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto com = component_holder_.CreateAndAddComponent();
			SetUpGameComponent(*com);
			return com;
		}
	private:
		//インターフェイスのオーバーライド
		//ゲームオブジェクトを作成
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id)override final;
		//ゲームオブジェクトを作成して有効化
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id)override final;
		//コンポーネントを型で取得
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		//コンポーネントを型ですべて取得
		std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;

		//自身の弱参照
		std::weak_ptr<GameObjectBase> this_weak_ptr_;
		//マネージャコネクション
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//シーンアクセサ
		utility::WeakPointer<core::SceneData> scene_data_;
		//コンポーネントホルダー
		GameObjectComponentHolder component_holder_;

		//コンポーネント関連の処理

		void SetUpGameComponent(GameObjectComponent& com);
		
		//イベントデリゲート
		utility::Delegate<void> activated_event_delegate_;
		utility::Delegate<void> inactivated_event_delegate_;
		utility::Delegate<void> disposed_event_delegate_;

		//イベント関数
		//初期化イベント
		virtual bool OnInitialized();
		//有効化イベント
		virtual bool OnActivated();
		//無効化イベント
		virtual bool OnInactivated();
		//破棄時イベント
		virtual bool OnDisposed();
	};
}