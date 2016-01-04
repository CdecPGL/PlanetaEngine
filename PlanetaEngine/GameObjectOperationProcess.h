#pragma once

#include "GameProcess.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace game_processes {
		class GameObjectOperationProcess : public game::GameProcess {
		public:
			using GameProcess::GameProcess;
			/**
			* @brief ����Ώۂ̃Q�[���I�u�W�F�N�g���Z�b�g����
			* @param �Q�[���I�u�W�F�N�g,�Ώۂ̃Q�[���I�u�W�F�N�g�j�����Ƀv���Z�X��j�����邩�ǂ���
			* @return �Z�b�g���ł�����
			*/
			bool Attach(GameObjectAccessorType& goa,bool dispose_on_target_disposed);
			/**
			* @brief ����Ώۂ̃Q�[���I�u�W�F�N�g����������
			* @return �������ł�����
			*/
			bool Detach();
			/**
			* @brief ����Ώۂ����Z�b�g����Ă��邩�ǂ���
			* @return �Z�b�g����Ă��邩
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
