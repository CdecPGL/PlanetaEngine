#pragma once

namespace planeta {
	namespace util {
		/*�l���w�肵���͈͂Ɏ��߂�*/
		template<typename T>
		constexpr T AdjustInRange(T value, T min, T max) {
			return (value < min) ? min : (value > max) ? max : value;
		}
		/*�l���w�肵���͈͂ɂ��邩�m�F����*/
		template<typename T>
		constexpr bool IsInRange(T value, T min, T max) {
			return min <= value && value <= max;
		}
	}
}