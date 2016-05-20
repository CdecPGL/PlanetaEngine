#include "TGameObjectOperation.h"
#include "SystemLog.h"
#include "IGameObject.h"

namespace planeta_engine {
	bool TGameObjectOperation::Attach(GameObjectAccessorType& goa, bool dispose_on_target_disposed) {
		if (is_target_attached_) {
			debug::SystemLog::instance().LogWarning("����Ώۂ̃Q�[���I�u�W�F�N�g�����łɃA�^�b�`����Ă��܂��B", __FUNCTION__);
			return false;
		}
		if (!AttachProc()) {
			debug::SystemLog::instance().LogWarning("�Q�[���I�u�W�F�N�g�̃A�^�b�`�Ɏ��s���܂����B", __FUNCTION__);
			return false;
		}
		dispose_on_target_disposed_ = dispose_on_target_disposed;
		RegisterEventHandler(goa);
		is_target_attached_ = true;
		return true;
	}

	bool TGameObjectOperation::Detach() {
		if (!is_target_attached_) {
			debug::SystemLog::instance().LogWarning("����Ώۂ̃Q�[���I�u�W�F�N�g���A�^�b�`����Ă��܂���B", __FUNCTION__);
			return false;
		}
		if (!DetachProc()) {
			debug::SystemLog::instance().LogWarning("�Q�[���I�u�W�F�N�g�̃f�^�b�`�Ɏ��s���܂����B", __FUNCTION__);
			return false;
		}
		is_target_attached_ = false;
		return true;
	}

	void TGameObjectOperation::RegisterEventHandler(GameObjectAccessorType& goa) {
		dispose_event_connection_ = goa->AddDisposedEventHandler(utility::CreateDelegateHandlerAdder([this]() {
			OnTargetDisposed();
			if (dispose_on_target_disposed_) {
				Dispose();
			}
		}));
		activate_event_connection_ = goa->AddActivatedEventHandler(utility::CreateDelegateHandlerAdder([this]() {
			OnTargetActivated();
		}));
		inactivate_event_connection_ = goa->AddInactivatedEventHandler(utility::CreateDelegateHandlerAdder([this]() {
			OnTargetInactivated();
		}));
	}

	void TGameObjectOperation::UnregisterEventHandler() {
		dispose_event_connection_.Remove();
		activate_event_connection_.Remove();
		inactivate_event_connection_.Remove();
	}
}