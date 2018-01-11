#pragma once

#include "planeta/core/Object.hpp"
#include "GameObjectComponentHolder.hpp"
#include "planeta/core/IGameObject.hpp"
#include "NonCopyable.hpp"
#include "planeta/core/NonOwingPointer.hpp"
#include "GameObjectManagerConnection.hpp"

namespace plnt {
	class JSONObject;
	class GameObjectComponent;
	class GOComponentGetter;
	namespace private_ {
		class ISceneInternal;

		/*! @brief GameObjectの具体的な実装を行うクラス。直接用いることはない。
		*/
		class GameObjectBase :public Object, public IGameObject, private util::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
		public:
			using Super = Object;
			GameObjectBase();
			~GameObjectBase();

			//インターフェイスのオーバーライド
			//有効化する
			void Activate()override final;
			//無効化する
			void Inactivate()override final;
			//破棄する
			void Dispose()override final;
			//自分のstd::shared_ptrを取得する。
			WeakPointer<IGameObject> GetPointer() override final;

			//状態を取得
			GameObjectState state()const override;
			//シーンへのアクセスを取得
			IScene& scene()override;

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
			//シーンをセット
			void SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal>& iscene);
			//コンポーネントリストからコンポーネントを追加し、シーンデータなどをセット
			bool AddAndSetUpComponents(const std::vector<std::string>& com_type_id_list);
			//boost::ptreeからコンポーネントを作成追加し、シーンデータなどをセット
			bool AddAndSetUpComponents(const boost::property_tree::ptree& pt);
		private:
			GameObjectState state_ = GameObjectState::Invalid;
			//コンポーネントを型で取得
			std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
			//コンポーネントを型ですべて取得
			//std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
			void SetUpAttachedTask_(const WeakPointer<Task>& task)override final;

			//マネージャコネクション
			std::unique_ptr<GameObjectManagerConnection> manager_connection_;
			//シーン内部用インターフェイス
			WeakPointer<private_::ISceneInternal> scene_internal_interface_;
			//コンポーネントホルダー
			GameObjectComponentHolder component_holder_;

			//コンポーネント関連の処理
			void SetSceneAndGODataToCOmponents_();
			void SetSceneAndGODataToComponent_(GameObjectComponent& com);
			//コンポーネントTypeIDからコンポーネントを追加。
			bool AddComponentsFromTypeIDList_(const std::vector<std::string>& com_type_id_list, std::vector<std::shared_ptr<GameObjectComponent>>& added_coms);
			//コンポーネントをPtreeから設定
			bool SetDataToComponentsFromPtree_(const std::vector<std::shared_ptr<GameObjectComponent>>& coms, const std::vector<std::shared_ptr<const boost::property_tree::ptree>>& pts);

			//アタッチされたタスク
			std::list<WeakPointer<Task>> attached_tasks_;
			//アタッチされたが存在しるか確認しつつ、タスクに処理を行う。存在しない場合はリストから外す。
			bool CheckAndApplyProcessToAttachedTask(const std::function<bool(Task&)>& proc);
		};
		PE_REFLECTABLE_CLASS(GameObjectBase);
	}
}
