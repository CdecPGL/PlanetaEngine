#pragma once

#include <functional>

namespace planeta {
	namespace core {
		class Screen;
		class ScreenChangeEffecter {
		public:
			bool ApplyEffect(Screen& screen) { return ApplyEffectProc(screen, internal_progress_); }
			/*エフェクトの進行度(0.0~1.0)*/
			void progress(double p);
			double progress()const { return progress_; }
			void reverse_flag(bool f);
			bool reverse_flag()const { return reverse_flag_; }
		private:
			//進行度
			double progress_ = 0.0;
			//内部進行度
			double internal_progress_ = 0.0;
			//進行度曲線
			std::function<double(double)> progress_curve_ = [](double v) { return v; };
			/*反転フラグ*/
			bool reverse_flag_ = false;

			void UpdateInternalProgress_();

			virtual bool ApplyEffectProc(Screen& screen,double progress) = 0;
		};
	}
}