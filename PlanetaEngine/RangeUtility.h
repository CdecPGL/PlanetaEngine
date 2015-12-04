#pragma once

namespace planeta_engine {
	namespace utility {
		/*値を指定した範囲に収める*/
		template<typename T>
		T AdjustInRange(T value, T min, T max) {
			if (value < min) { return min; } 
			else if (value > max) { return max; }
		}
		/*値が指定した範囲にあるか確認する*/
		template<typename T>
		bool IsInRange(T value, T min, T max) {
			return min <= value && value <= max;
		}
	}
}