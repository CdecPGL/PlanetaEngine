#pragma once

#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace utility {
		/**
		* @brief �C�x���g�Ď��N���X�{�́B
		*/
		class EventMonitor final{
		public:
			/**
			* @brief �f�t�H���g�R���X�g���N�^
			*/
			EventMonitor() {}
			/**
			* @brief �R���X�g���N�^
			* @param (dlgt) �Ď�����C�x���g�̃f���Q�[�g
			*/
			template<typename EventArgType>
			EventMonitor(WeakPointerDelegate<EventArgType>& dlgt) { SetTarget(dlgt); }
			/**
			* @fn
			* �f�X�g���N�^�B�C�x���g�f���Q�[�g�ւ̓o�^����������B
			* @brief �f�X�g���N�^
			* @param (dlgt) �Ď�����C�x���g�̃f���Q�[�g
			*/
			~EventMonitor() {
				delegate_connection_.Remove();
			}
			/**
			* @brief �C�x���g�����������m�F����B
			* @return �������Ă�����true,���Ă��Ȃ�������false
			*/
			bool CheckEventCalled()const { return is_event_called_; }
			/**
			* @brief �Ď��Ώۂ̃C�x���g�f���Q�[�g���Z�b�g
			*/
			template<typename EventArgType>
			void SetTarget(WeakPointerDelegate<EventArgType>& dlgt) {
				delegate_connection_.Remove(); //�����̐ڑ��͐؂�
				delegate_connection_ = dlgt.Add([this](WeakPointerDelegate<EventArgType>::HandlerParamType) {
					delegate_connection_.Remove();
					is_event_called_ = true;
				});
				is_event_called_ = false;
			}
			void SetTarget(WeakPointerDelegate<void>& dlgt) {
				delegate_connection_.Remove(); //�����̐ڑ��͐؂�
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