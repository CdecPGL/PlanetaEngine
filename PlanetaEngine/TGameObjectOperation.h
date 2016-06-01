#pragma once

#include "Task.h"
#include "Delegate.h"

namespace planeta {
	class TGameObjectOperation : public Task {
	public:
		using Task::Task;
		/**
		* @brief ����Ώۂ̃Q�[���I�u�W�F�N�g���Z�b�g����
		* @param �Q�[���I�u�W�F�N�g,�Ώۂ̃Q�[���I�u�W�F�N�g�j�����Ƀv���Z�X��j�����邩�ǂ���
		* @return �Z�b�g���ł�����
		*/
		bool Attach(const GameObjectAccessorType& goa, bool dispose_on_target_disposed);
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
