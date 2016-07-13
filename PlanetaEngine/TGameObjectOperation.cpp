#include "TGameObjectOperation.h"
#include "SystemLog.h"
#include "IGameObject.h"

namespace planeta {
	bool TGameObjectOperation::Attach(const GameObjectAccessorType& goa, bool dispose_on_target_disposed) {
		if (is_target_attached_) {
			PE_LOG_WARNING("����Ώۂ̃Q�[���I�u�W�F�N�g�����łɃA�^�b�`����Ă��܂��B");
			return false;
		}
		if (!OnGameObjectAttached()) {
			PE_LOG_WARNING("�Q�[���I�u�W�F�N�g�̃A�^�b�`�Ɏ��s���܂����B");
			return false;
		}
		dispose_on_target_disposed_ = dispose_on_target_disposed;
		RegisterEventHandler(*goa);
		is_target_attached_ = true;
		return true;
	}

	bool TGameObjectOperation::Detach() {
		if (!is_target_attached_) {
			PE_LOG_WARNING("����Ώۂ̃Q�[���I�u�W�F�N�g���A�^�b�`����Ă��܂���B");
			return false;
		}
		if (!OnGameObjectDetached()) {
			PE_LOG_WARNING("�Q�[���I�u�W�F�N�g�̃f�^�b�`�Ɏ��s���܂����B");
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