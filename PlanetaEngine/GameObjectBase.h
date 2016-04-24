#pragma once

#include "GameObjectComponentHolder.h"
#include "IGameObject.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class GameObjectComponent;
	class GameObjectBase : public IGameObject,private utility::NonCopyable<GameObjectBase> {
	public:
		//インターフェイスのオーバーライド
		//有効化する
		bool Activate()override final;
		//無効化する
		bool Inactivate()override final;
		//破棄する
		void Dispose()override final;
		//自分のstd::shared_ptrを取得する。
		std::shared_ptr<IGameObject> GetSharedPointer()const override final;

		//システム用関数(Managerから呼び出される)
		//有効化時の処理
		bool ProcessActivation();
		//無効化時の処理
		bool ProcessInactivation();
		//破棄時の処理
		bool ProcessDisposDisposal();

		//静的関数
		//GameObjectの作成関数。自分のスマートポインタを保持するためにこの関数を介する必要がある。
		template<class T>
		static std::shared_ptr<GameObjectBase> Create() {
			static_assert(std::is_base_of<GameObjectBase, T>::value == true, "T must derive GameOBjectBase");
			auto ptr = std::make_shared<T>();
			ptr->this_weak_ptr_ = ptr;
			return std::move(ptr);
		}
	protected:
		//コンポーネントを作成、追加する。
		template<class ComT>
		utility::WeakPointer<ComT> CreateAndAddComponent() {
			return component_holder_.CreateAndAddComponent();
		}
		//コンポーネントを型で取得する。
		template<class ComT>
		utility::WeakPointer<ComT> GetComponent() {
			return component_holder_.GetComponent<ComT>();
		}
		//コンポーネントを型で全て取得する。
		template<class ComT>
		std::vector<utility::WeakPointer<ComT>> GetAllComponents() {
			return std::move(component_holder_.GetAllComponents<ComT>());
		}
	private:
		//自身の弱参照
		std::weak_ptr<GameObjectBase> this_weak_ptr_;
		//コンポーネントホルダー
		GameObjectComponentHolder component_holder_;

		//コンポーネント関連の処理
		

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