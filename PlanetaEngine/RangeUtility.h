#pragma once

namespace planeta_engine {
	namespace utility {
		/*値を指定した範囲に収める*/
		template<typename T>
		constexpr T AdjustInRange(T value, T min, T max) {
			return (value < min) ? min : (value > max) ? max : value;
		}
		/*値が指定した範囲にあるか確認する*/
		template<typename T>
		constexpr bool IsInRange(T value, T min, T max) {
			return min <= value && value <= max;
		}
	}
}