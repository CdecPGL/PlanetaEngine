#include "GameObjectOperationProcess.h"
#include "SystemLog.h"
#include "IGameObjectAccessor.h"

namespace planeta_engine {
	namespace game_processes {

		bool GameObjectOperationProcess::Attach(GameObjectAccessorType& goa, bool dispose_on_target_disposed) {
			if (is_target_attached_) {
				debug::SystemLog::instance().LogWarning("操作対象のゲームオブジェクトがすでにアタッチされています。", __FUNCTION__);
				return false;
			}
			if (!AttachProc()) {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクトのアタッチに失敗しました。", __FUNCTION__);
				return false;
			}
			dispose_on_target_disposed_ = dispose_on_target_disposed;
			RegisterEventHandler(goa);
			is_target_attached_ = true;
			return true;
		}

		bool GameObjectOperationProcess::Detach() {
			if (!is_target_attached_) {
				debug::SystemLog::instance().LogWarning("操作対象のゲームオブジェクトがアタッチされていません。", __FUNCTION__);
				return false;
			}
			if (!DetachProc()) {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクトのデタッチに失敗しました。", __FUNCTION__);
				return false;
			}
			is_target_attached_ = false;
			return true;
		}

		void GameObjectOperationProcess::RegisterEventHandler(GameObjectAccessorType& goa) {
			dispose_event_connection_ = goa->disposed_event.Add([this]() {
				OnTargetDisposed();
				if (dispose_on_target_disposed_) {
					Dispose();
				}
			});
			activate_event_connection_ = goa->activated_event.Add([this]() {
				OnTargetActivated();
			});
			inactivate_event_connection_ = goa->inactivated_event.Add([this]() {
				OnTargetInactivated();
			});
		}

		void GameObjectOperationProcess::UnregisterEventHandler() {
			dispose_event_connection_.Remove();
			activate_event_connection_.Remove();
			inactivate_event_connection_.Remove();
		}

	}
}