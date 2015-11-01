#pragma once

#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace utility {
		/**
		* @brief イベント監視クラス本体。
		*/
		class EventMonitor final{
		public:
			/**
			* @brief デフォルトコンストラクタ
			*/
			EventMonitor() {}
			/**
			* @brief コンストラクタ
			* @param (dlgt) 監視するイベントのデリゲート
			*/
			template<typename EventArgType>
			EventMonitor(WeakPointerDelegate<EventArgType>& dlgt) { SetTarget(dlgt); }
			/**
			* @fn
			* デストラクタ。イベントデリゲートへの登録を解除する。
			* @brief デストラクタ
			* @param (dlgt) 監視するイベントのデリゲート
			*/
			~EventMonitor() {
				delegate_connection_.Remove();
			}
			/**
			* @brief イベントが発したか確認する。
			* @return 発生していたらtrue,していなかったらfalse
			*/
			bool CheckEventCalled()const { return is_event_called_; }
			/**
			* @brief 監視対象のイベントデリゲートをセット
			*/
			template<typename EventArgType>
			void SetTarget(WeakPointerDelegate<EventArgType>& dlgt) {
				delegate_connection_.Remove(); //既存の接続は切る
				delegate_connection_ = dlgt.Add([this](WeakPointerDelegate<EventArgType>::HandlerParamType) {
					delegate_connection_.Remove();
					is_event_called_ = true;
				});
				is_event_called_ = false;
			}
			void SetTarget(WeakPointerDelegate<void>& dlgt) {
				delegate_connection_.Remove(); //既存の接続は切る
				delegate_connection_ = dlgt.Add([this]() {
					delegate_connection_.Remove();
					is_event_called_ = true;
				});
				is_event_called_ = false;
			}

		private:
			bool is_event_called_ = false;
			WeakPointerDelegateConnection delegate_connection_;
		};
	}
}