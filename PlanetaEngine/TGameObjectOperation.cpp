#include "TGameObjectOperation.h"
#include "SystemLog.h"
#include "IGameObject.h"

namespace planeta_engine {
	bool TGameObjectOperation::Attach(const GameObjectAccessorType& goa, bool dispose_on_target_disposed) {
		if (is_target_attached_) {
			debug::SystemLog::instance().LogWarning("操作対象のゲームオブジェクトがすでにアタッチされています。", __FUNCTION__);
			return false;
		}
		if (!OnGameObjectAttached()) {
			debug::SystemLog::instance().LogWarning("ゲームオブジェクトのアタッチに失敗しました。", __FUNCTION__);
			return false;
		}
		dispose_on_target_disposed_ = dispose_on_target_disposed;
		RegisterEventHandler(*goa);
		is_target_attached_ = true;
		return true;
	}

	bool TGameObjectOperation::Detach() {
		if (!is_target_attached_) {
			debug::SystemLog::instance().LogWarning("操作対象のゲームオブジェクトがアタッチされていません。", __FUNCTION__);
			return false;
		}
		if (!OnGameObjectDetached()) {
			debug::SystemLog::instance().LogWarning("ゲームオブジェクトのデタッチに失敗しました。", __FUNCTION__);
			return false;
		}
		is_target_attached_ = false;
		return true;
	}

	void TGameObjectOperation::RegisterEventHandler(IGameObject& goa) {
		dispose_event_connection_ = goa.AddDisposedEventHandler(util::CreateDelegateHandlerAdder([this]() {
			OnTargetDisposed();
			if (dispose_on_target_disposed_) {
				Dispose();
			}
		}));
		activate_event_connection_ = goa.AddActivatedEventHandler(util::CreateDelegateHandlerAdder([this]() {
			OnTargetActivated();
		}));
		inactivate_event_connection_ = goa.AddInactivatedEventHandler(util::CreateDelegateHandlerAdder([this]() {
			OnTargetInactivated();
		}));
	}

	void TGameObjectOperation::UnregisterEventHandler() {
		dispose_event_connection_.Remove();
		activate_event_connection_.Remove();
		inactivate_event_connection_.Remove();
	}
}