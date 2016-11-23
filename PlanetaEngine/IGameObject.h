#pragma once

#include <type_traits>
#include <memory>

#include "Signal.h"
#include "ITaskManager.h"
#include "TaskSlot.h"
#include "IScene.h"

namespace planeta {
	class Task;
	class GameObjectComponent;
	class Task;
	/*! ゲームオブジェクトの状態*/
	enum class GameObjectState { Invalid, Inactive, Active, Initializing, Inactivating, Activating };
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
		virtual WeakPointer<IGameObject> GetPointer()= 0;
		//! コンポーネントを型で取得する
		template<class ComT>
		WeakPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//! 特定型のコンポーネントを持っているか確認する
		template<class ComT>
		bool HasComponentType()const { return false; }
		//! コンポーネントを型ですべて取得する

		//! コンポーネントをコンポーネント型IDで取得する

		//! コンポーネントをコンポーネント型IDで全て取得する

		//! 特定のコンポーネント型IDのコンポーネントを持っているか確認

		/*! @brief タスクをアタッチする

			テンプレート引数で指定した型のタスクを作成し、アタッチする。
			ゲームオブジェクトのアタッチされたタスクの寿命はそのゲームオブジェクトと同じになり、ゲームオブジェクトの無効化有効化に合わせて停止、再開する。
		*/
		template<class T>
		WeakPointer<T> CreateAndAttachTask(TaskSlot slot) {
			static_assert(std::is_base_of<Task, T>::value == true, "T must derive Task");
			auto task = scene().task_manager().CreateTask<T>(slot);
			if (task == nullptr) { return nullptr; }
			SetUpAttachedTask_(task);
			return task;
		}
		//! ゲームオブジェクトの状態を取得する
		virtual GameObjectState state()const = 0;

		/*イベント*/
		/*! 有効化イベント*/
		Signal<void()> activated;
		/*! 無効化イベント*/
		Signal<void()> inactivated;
		/*! 破棄イベント*/
		Signal<void()> disposed;
	protected:
		virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
		virtual void SetUpAttachedTask_(const WeakPointer<Task>& task) = 0;
		virtual IScene& scene() = 0;
	};
}
