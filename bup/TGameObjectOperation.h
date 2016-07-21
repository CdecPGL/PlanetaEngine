#pragma once

#include "Task.h"
#include "Delegate.h"

namespace planeta {
	class TGameObjectOperation : public Task {
	public:
		using Task::Task;
		/**
		* @brief 操作対象のゲームオブジェクトをセットする
		* @param ゲームオブジェクト,対象のゲームオブジェクト破棄時にプロセスを破棄するかどうか
		* @return セットができたか
		*/
		bool Attach(const GameObjectAccessorType& goa, bool dispose_on_target_disposed);
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
		void RegisterEventHandler(IGameObject& goa);
		void UnregisterEventHandler();
		virtual bool OnGameObjectAttached() { return true; };
		virtual bool OnGameObjectDetached() { return true; };
		virtual void OnTargetDisposed() {}
		virtual void OnTargetActivated() {}
		virtual void OnTargetInactivated() {}
		util::DelegateConnection dispose_event_connection_;
		util::DelegateConnection activate_event_connection_;
		util::DelegateConnection inactivate_event_connection_;
	};
}
