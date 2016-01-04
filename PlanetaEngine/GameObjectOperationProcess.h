#pragma once

#include "GameProcess.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace game_processes {
		class GameObjectOperationProcess : public game::GameProcess {
		public:
			using GameProcess::GameProcess;
			/**
			* @brief 操作対象のゲームオブジェクトをセットする
			* @param ゲームオブジェクト,対象のゲームオブジェクト破棄時にプロセスを破棄するかどうか
			* @return セットができたか
			*/
			bool Attach(GameObjectAccessorType& goa,bool dispose_on_target_disposed);
			/**
			* @brief 操作対象のゲームオブジェクトを解除する
			* @return 解除ができたか
			*/
			bool Detach();
			/**
			* @brief 操作対象ががセットされているかどうか
			* @return セットされているか
			*/
			bool is_taget_attached()const { return is_target_attached_; }
		private:
			bool dispose_on_target_disposed_ = false;
			bool is_target_attached_ = false;
			void RegisterEventHandler(GameObjectAccessorType& goa);
			void UnregisterEventHandler();
			virtual bool AttachProc() = 0;
			virtual bool DetachProc() = 0;
			virtual void OnTargetDisposed() {}
			virtual void OnTargetActivated() {}
			virtual void OnTargetInactivated() {}
			utility::WeakPointerDelegateConnection dispose_event_connection_;
			utility::WeakPointerDelegateConnection activate_event_connection_;
			utility::WeakPointerDelegateConnection inactivate_event_connection_;
		};
	}
}
