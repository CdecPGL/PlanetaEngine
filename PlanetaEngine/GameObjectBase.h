#pragma once

#include "Object.h"
#include "GameObjectComponentHolder.h"
#include "IGameObject.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"
#include "GameObjectManagerConnection.h"

namespace planeta {
	class JSONObject;
	class GameObjectComponent;
	class GOComponentGetter;
	namespace private_ {
		struct SceneData;

		/*! @brief GameObjectの具体的な実装を行うクラス。直接用いることはない。
		*/
		class GameObjectBase :public Object, public IGameObject, private util::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
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
			WeakPointer<IGameObject> GetPointer() override final;
			//有効化イベントハンドラ登録
			DelegateConnection AddActivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder)override final;
			//無効化イベントハンドラ登録
			DelegateConnection AddInactivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder)override final;
			//破棄イベントハンドラ登録
			DelegateConnection AddDisposedEventHandler(DelegateHandlerAdder<void>&& hander_adder)override final;
			//状態を取得
			GameObjectState state()const override;

			//システム用関数(Managerから呼び出される｡GameObjectクラスで隠ぺいする)
			//クローン時の処理
			bool ProcessClonation(const std::shared_ptr<GameObjectBase>& dst);
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
			void SetSceneData(const WeakPointer<private_::SceneData>& scene_data);
			//コンポーネントリストからコンポーネントを追加し、シーンデータなどをセット
			bool AddAndSetUpComponents(const std::vector<std::string>& com_type_id_list);
			//boost::ptreeからコンポーネントを作成追加し、シーンデータなどをセット
			bool AddAndSetUpComponents(const boost::property_tree::ptree& pt);
		private:
			GameObjectState state_ = GameObjectState::Invalid;
			//インターフェイスのオーバーライド
			GameObjectManagerPublicInterface& game_object_manager()override;
			//コンポーネントを型で取得
			std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
			//コンポーネントを型ですべて取得
			//std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
			TaskManagerPublicInterface& RefTaskManagerInterface_()override final;
			void SetUpAttachedTask_(const WeakPointer<Task>& task)override final;

			//マネージャコネクション
			std::unique_ptr<GameObjectManagerConnection> manager_connection_;
			//シーンアクセサ
			WeakPointer<private_::SceneData> scene_data_;
			//コンポーネントホルダー
			GameObjectComponentHolder component_holder_;

			//コンポーネント関連の処理
			void SetSceneAndGODataToCOmponents_();
			void SetSceneAndGODataToComponent_(GameObjectComponent& com);
			//コンポーネントTypeIDからコンポーネントを追加。("エイリアス:タイプID"とすることでエイリアスを指定可能。そうでない場合はTypeIDがエイリアスとなる)
			bool AddComponentsFromTypeIDList_(const std::vector<std::string>& com_type_id_list, std::vector<std::shared_ptr<GameObjectComponent>>& added_coms);
			//コンポーネントをPtreeから設定
			bool SetDataToComponentsFromPtree_(const std::vector<std::shared_ptr<GameObjectComponent>>& coms, const std::vector<std::shared_ptr<const boost::property_tree::ptree>>& pts);

			//アタッチされたタスク
			std::list<WeakPointer<Task>> attached_tasks_;
			//アタッチされたが存在しるか確認しつつ、タスクに処理を行う。存在しない場合はリストから外す。
			bool CheckAndApplyProcessToAttachedTask(const std::function<bool(Task&)>& proc);

			//イベントデリゲート
			Delegate<void> activated_event_delegate_;
			Delegate<void> inactivated_event_delegate_;
			Delegate<void> disposed_event_delegate_;
		};
		PE_REFLECTABLE_CLASS(GameObjectBase);
	}
}
