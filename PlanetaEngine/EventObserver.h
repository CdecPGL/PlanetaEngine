#pragma once

#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace utility {
		/**
		* @brief イベント監視クラス。
		*/
		template<typename EventArgType>
		class EventObserver final{
		public:
			/**
			* @brief コンストラクタ
			* @param (dlgt) 監視するイベントのデリゲート
			*/
			EventObserver(WeakPointerDelegate<EventArgType>& dlgt) {
				delegate_connection_ = dlgt.Add([this]() {
					delegate_connection_.Remove();
					is_event_called_ = true; 
				});
			}
			/**
			* @fn
			* デストラクタ。イベントデリゲートへの登録を解除する。
			* @brief デストラクタ
			* @param (dlgt) 監視するイベントのデリゲート
			*/
			~EventObserver() {
				delegate_connection_.Remove();
			}
			/**
			* @brief イベントが発したか確認する。
			* @return 発生していたらtrue,していなかったらfalse
			*/
			bool CheckEventCalled()const { return is_event_called_; }
		private:
			bool is_event_called_ = false;
			WeakPointerDelegateConnection delegate_connection_;
		};
	}
}