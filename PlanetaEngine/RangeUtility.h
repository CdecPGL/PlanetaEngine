#pragma once

namespace planeta_engine {
	namespace utility {
		/*�l���w�肵���͈͂Ɏ��߂�*/
		template<typename T>
		T AdjustInRange(T value, T min, T max) {
			if (value < min) { return min; } 
			else if (value > max) { return max; }
		}
		/*�l���w�肵���͈͂ɂ��邩�m�F����*/
		template<typename T>
		bool IsInRange(T value, T min, T max) {
			return min <= value && value <= max;
		}
	}
}