#pragma once

#include <type_traits>
#include <memory>
#include "GameObjectInterface.h"
#include "Delegate.h"
#include "TaskSlot.h"

namespace planeta_engine {
	class Task;
	class GameObjectComponent;
	class IGameObject {
	public:
		//有効化
		virtual bool Activate() = 0;
		//無効化
		virtual bool Inactivate() = 0;
		//破棄
		virtual void Dispose() = 0;
		//自身のstd::shared_ptrを取得
		virtual std::shared_ptr<IGameObject> GetSharedPointer()const = 0;
		//コンポーネントを型で取得する。
		template<class ComT>
		utility::WeakPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//指定タイプのゲームオブジェクトインターフェイス取得
		template<class GOI>
		std::shared_ptr<GOI> GetInterface()const {
			static_assert(std::is_base_of<GameObjectInterface, GOI>::value == true, "GOI must drive GameObjectInterface.");
			auto ptr = std::dynamic_pointer_cast<GOI>(GetSharedPointer());
			return ptr;
		}
		//指定タイプのゲームオブジェクトインターフェイスを持っているか確認
		template<class GOI>
		bool HasInterface()const {
			static_assert(std::is_base_of<GameObjectInterface, GOI>::value == true, "GOI must drive GameObjectInterface.");
			return GetInterface<GOI>() != nullptr;
		}
		//タスクをアタッチ
		template<class T>
		utility::WeakPointer<T> CreateAndAttachTask(TaskSlot slot);
		//有効化イベントハンドラ登録
		virtual utility::DelegateConnection AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//無効化イベントハンドラ登録
		virtual utility::DelegateConnection AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//破棄イベントハンドラ登録
		virtual utility::DelegateConnection AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
	protected:
		virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
	};
}
