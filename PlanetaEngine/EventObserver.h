#pragma once

#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace utility {
		/**
		* @brief �C�x���g�Ď��N���X�B
		*/
		template<typename EventArgType>
		class EventObserver final{
		public:
			/**
			* @brief �R���X�g���N�^
			* @param (dlgt) �Ď�����C�x���g�̃f���Q�[�g
			*/
			EventObserver(WeakPointerDelegate<EventArgType>& dlgt) {
				delegate_connection_ = dlgt.Add([this]() {
					delegate_connection_.Remove();
					is_event_called_ = true; 
				});
			}
			/**
			* @fn
			* �f�X�g���N�^�B�C�x���g�f���Q�[�g�ւ̓o�^����������B
			* @brief �f�X�g���N�^
			* @param (dlgt) �Ď�����C�x���g�̃f���Q�[�g
			*/
			~EventObserver() {
				delegate_connection_.Remove();
			}
			/**
			* @brief �C�x���g�����������m�F����B
			* @return �������Ă�����true,���Ă��Ȃ�������false
			*/
			bool CheckEventCalled()const { return is_event_called_; }
		private:
			bool is_event_called_ = false;
			WeakPointerDelegateConnection delegate_connection_;
		};
	}
}