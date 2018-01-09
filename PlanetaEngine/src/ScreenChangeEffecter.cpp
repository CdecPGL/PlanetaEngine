#include "ScreenChangeEffecter.hpp"
#include "RangeUtility.hpp"

namespace planeta {
	namespace private_ {

		void ScreenChangeEffecter::progress(double p) {
			progress_ = util::AdjustInRange(p, 0.0, 1.0);
			UpdateInternalProgress_();
		}

		void ScreenChangeEffecter::reverse_flag(bool f) {
			reverse_flag_ = f;
			UpdateInternalProgress_();
		}

		void ScreenChangeEffecter::UpdateInternalProgress_() {
			internal_progress_ = util::AdjustInRange(progress_curve_(reverse_flag_ ? (1.0 - progress_) : progress_), 0.0, 1.0);
		}

	}
}