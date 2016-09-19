#pragma once

#include <type_traits>
#include <memory>
#include "GameObjectInterface.h"
#include "TaskManagerPublicInterface.h"
#include "Delegate.h"
#include "TaskSlot.h"

namespace planeta {
	class Task;
	class GameObjectComponent;
	class TGameObjectOperation;
	/*! @brief ゲームオブジェクトへのアクセスを提供するインターフェイスクラス
	*/
	class IGameObject {
	public:
		//! ゲームオブジェクトをシーン内で有効化する。戻り値は有効化の成否を示す。  
		virtual bool Activate() = 0;
		//! ゲームオブジェクトをシーン内で無効化する。戻り値は無効化の成否を示す。  
		virtual bool Inactivate() = 0;
		//! ゲームオブジェクトをシーンから破棄する
		virtual void Dispose() = 0;
		//! 自身のstd::shared_ptrを取得する
		virtual std::shared_ptr<IGameObject> GetSharedPointer()= 0;
		//! コンポーネントを型で取得する。乱用禁止。できれば代わりにGetInterface()を使う。
		template<class ComT>
		WeakPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		/*! @brief 指定タイプのゲームオブジェクトインターフェイス取得
			
			テンプレート引数で指定した型か、その子クラスのインターフェイスを持っていれば返す。持っていない場合はnullptrを返す。
		*/
		template<class GOI>
		std::shared_ptr<GOI> GetInterface() {
			static_assert(std::is_base_of<GameObjectInterface<GOI>, GOI>::value == true, "GOI must drive GameObjectInterface.");
			auto ptr = std::dynamic_pointer_cast<GOI>(GetSharedPointer());
			return ptr;
		}
		/*! @brief 指定タイプのゲームオブジェクトインターフェイスを持っているか確認

			テンプレート引数で指定した型か、その子クラスのインターフェイスを持っているかどうかを返す。
		*/
		template<class GOI>
		bool HasInterface() {
			static_assert(std::is_base_of<GameObjectInterface<GOI>, GOI>::value == true, "GOI must drive GameObjectInterface.");
			return GetInterface<GOI>() != nullptr;
		}
		/*! @brief タスクをアタッチする

			テンプレート引数で指定した型のタスクを作成し、アタッチする。
			TはTGameObjectOperationを継承したクラス。
			ゲームオブジェクトのアタッチされたタスクの寿命はそのゲームオブジェクトと同じになり、ゲームオブジェクトの無効化有効化に合わせて停止、再開する。
		*/
		template<class T>
		WeakPointer<T> CreateAndAttachTask(TaskSlot slot) {
			static_assert(std::is_base_of<TGameObjectOperation, T>::value == true, "T must derive TGameObjectOperation");
			auto task = std::make_shared<T>();
			if (!RefTaskManagerInterface_().RegisterTask(task, slot)) { return nullptr; }
			SetUpAttachedTask_(*task);
			return task;
		}
		//! 有効化イベントハンドラを登録する
		virtual DelegateConnection AddActivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder) = 0;
		//! 無効化イベントハンドラを登録する
		virtual DelegateConnection AddInactivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder) = 0;
		//! 破棄イベントハンドラを登録する
		virtual DelegateConnection AddDisposedEventHandler(DelegateHandlerAdder<void>&& hander_adder) = 0;
	protected:
		virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
		virtual TaskManagerPublicInterface& RefTaskManagerInterface_() = 0;
		virtual void SetUpAttachedTask_(TGameObjectOperation& task) = 0;
	};
}
