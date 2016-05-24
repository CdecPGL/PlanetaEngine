#pragma once
#include "SingletonTemplate.h"
#include "PETime.h"

namespace planeta_engine {
	namespace debug {
		//�N������̌o�߃t���[���ƌo�ߎ��Ԃ��J�E���g����(�X�^�e�B�b�N�V���O���g��)
		class SystemCounter : public utility::SingletonTemplate<SystemCounter> {
			friend utility::SingletonTemplate<SystemCounter>;
		public:
			bool Initialize()override;
			bool Finalize()override { return true; }
			void IncrementFrameCount() { ++_frame_count; }
			size_t GetCurrentFrameCount()const { return _frame_count; }
			const utility::Time GetCurrentTimeCount()const { return utility::Time::GetCurrentTime() - _start_time; }
		private:
			size_t _frame_count = 0;
			utility::Time _start_time;
		};
	}
}
