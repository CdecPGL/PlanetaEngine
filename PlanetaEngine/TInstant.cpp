#include "TInstant.h"

namespace planeta_engine {
	void TInstant::Update() {
		function_();
		if (number_of_times_implemented_ > 0) {
			++count_of_times_implemented_;
			if (number_of_times_implemented_ >= count_of_times_implemented_) {
				//���s�񐔂ɒB�����̂Ńv���Z�X���폜����
				Dispose();
			}
		}
	}
}