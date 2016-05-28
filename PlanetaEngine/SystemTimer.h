#pragma once
#include "SingletonTemplate.h"
#include "PETime.h"

namespace planeta_engine {
	namespace debug {
		//�N������̌o�߃t���[���ƌo�ߎ��Ԃ��J�E���g����(�X�^�e�B�b�N�V���O���g��)
		class SystemTimer : public utility::SingletonTemplate<SystemTimer> {
			friend utility::SingletonTemplate<SystemTimer>;
		public:
			bool Initialize()override;
			void Finalize()override {}

			void IncrementFrameCount() { ++_frame_count; }
			size_t GetCurrentFrameCount()const { return _frame_count; }
			const utility::Time GetCurrentTimeCount()const;
		private:
			SystemTimer() = default;
			size_t _frame_count = 0;
			utility::Time _start_time;
		};
	}
}
