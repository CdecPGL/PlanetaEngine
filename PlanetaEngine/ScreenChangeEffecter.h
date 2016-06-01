#pragma once

#include <functional>

namespace planeta {
	namespace core {
		class Screen;
		class ScreenChangeEffecter {
		public:
			bool ApplyEffect(Screen& screen) { return ApplyEffectProc(screen, internal_progress_); }
			/*�G�t�F�N�g�̐i�s�x(0.0~1.0)*/
			void progress(double p);
			double progress()const { return progress_; }
			void reverse_flag(bool f);
			bool reverse_flag()const { return reverse_flag_; }
		private:
			//�i�s�x
			double progress_ = 0.0;
			//�����i�s�x
			double internal_progress_ = 0.0;
			//�i�s�x�Ȑ�
			std::function<double(double)> progress_curve_ = [](double v) { return v; };
			/*���]�t���O*/
			bool reverse_flag_ = false;

			void UpdateInternalProgress_();

			virtual bool ApplyEffectProc(Screen& screen,double progress) = 0;
		};
	}
}