#include "GameObjectOperationProcess.h"
#include "SystemLog.h"
#include "IGameObjectAccessor.h"

namespace planeta_engine {
	namespace game_processes {

		bool GameObjectOperationProcess::Attach(GameObjectAccessorType& goa, bool dispose_on_target_disposed) {
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

		bool GameObjectOperationProcess::Detach() {
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