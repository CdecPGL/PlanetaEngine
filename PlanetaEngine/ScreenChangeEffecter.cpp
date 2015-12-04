#include "ScreenChangeEffecter.h"
#include "RangeUtility.h"

namespace planeta_engine {
	namespace core {

		void ScreenChangeEffecter::progress(double p) {
			progress_ = utility::AdjustInRange(p, 0.0, 1.0);
			UpdateInternalProgress_();
		}

		void ScreenChangeEffecter::reverse_flag(bool f) {
			reverse_flag_ = f;
			UpdateInternalProgress_();
		}

		void ScreenChangeEffecter::UpdateInternalProgress_() {
			internal_progress_ = utility::AdjustInRange(progress_curve_(reverse_flag_ ? (1.0 - progress_) : progress_), 0.0, 1.0);
		}

	}
}