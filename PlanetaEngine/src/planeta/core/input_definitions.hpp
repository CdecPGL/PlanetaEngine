﻿#pragma once

#include <cstdint>

namespace plnt {
	namespace button {
		using type = uint32_t;
		constexpr type up = 1,
		               down = 2,
		               left = 4,
		               right = 8,
		               a = 16,
		               b = 32,
		               x = 64,
		               y = 128,
		               l = 256,
		               r = 512,
		               start = 1024,
		               error = 1ul << 31;
	};

	namespace pad {
		using type = uint32_t;
		constexpr type up = 1ul << 0,
		               down = 1ul << 1,
		               left = 1ul << 2,
		               right = 1ul << 3,
		               input_1 = 1ul << 4,
		               input_2 = 1ul << 5,
		               input_3 = 1ul << 6,
		               input_4 = 1ul << 7,
		               input_5 = 1ul << 8,
		               input_6 = 1ul << 9,
		               input_7 = 1ul << 10,
		               input_8 = 1ul << 11,
		               input_9 = 1ul << 12,
		               input_10 = 1ul << 13,
		               input_11 = 1ul << 14,
		               input_12 = 1ul << 15,
		               input_13 = 1ul << 16,
		               input_14 = 1ul << 17,
		               input_15 = 1ul << 18,
		               input_16 = 1ul << 19,
		               input_17 = 1ul << 20,
		               input_18 = 1ul << 21,
		               input_19 = 1ul << 22,
		               input_20 = 1ul << 23,
		               pov_up = 1ul << 24,
		               pov_down = 1ul << 25,
		               pov_left = 1ul << 26,
		               pov_right = 1ul << 27,
		               error = 1ul << 31;
	};

	namespace keyboard {
		using type = uint64_t;
		constexpr type a = 1 << 0,
		               b = 1ull << 1,
		               c = 1ull << 2,
		               d = 1ull << 3,
		               e = 1ull << 4,
		               f = 1ull << 5,
		               g = 1ull << 6,
		               h = 1ull << 7,
		               i = 1ull << 8,
		               j = 1ull << 9,
		               k = 1ull << 10,
		               l = 1ull << 11,
		               m = 1ull << 12,
		               n = 1ull << 13,
		               o = 1ull << 14,
		               p = 1ull << 15,
		               q = 1ull << 16,
		               r = 1ull << 17,
		               s = 1ull << 18,
		               t = 1ull << 19,
		               u = 1ull << 20,
		               v = 1ull << 21,
		               w = 1ull << 22,
		               x = 1ull << 23,
		               y = 1ull << 24,
		               z = 1ull << 25,
		               space = 1ull << 26,
		               l_shift = 1ull << 27,
		               enter = 1ull << 28,
		               l_control = 1ull << 29,
		               num_0 = 1ull << 30,
		               num_1 = 1ull << 31,
		               num_2 = 1ull << 32,
		               num_3 = 1ull << 33,
		               num_4 = 1ull << 34,
		               num_5 = 1ull << 35,
		               num_6 = 1ull << 36,
		               num_7 = 1ull << 37,
		               num_8 = 1ull << 38,
		               num_9 = 1ull << 39,
		               f1 = 1ull << 40,
		               f2 = 1ull << 41,
		               f3 = 1ull << 42,
		               f4 = 1ull << 43,
		               f5 = 1ull << 44,
		               f6 = 1ull << 45,
		               f7 = 1ull << 46,
		               f8 = 1ull << 47,
		               f9 = 1ull << 48,
		               f10 = 1ull << 49,
		               f11 = 1ull << 50,
		               f12 = 1ull << 51,
		               esc = 1ull << 52,
		               r_shift = 1ull << 53,
		               r_control = 1ull << 54,
		               up = 1ull << 55,
		               down = 1ull << 56,
		               left = 1ull << 57,
		               right = 1ull << 58,
		               error = 1ull << 63;
	};

	namespace mouse_button {
		using type = uint32_t;
		constexpr type left = 1,
		               right = 2,
		               middle = 4,
		               input_4 = 8,
		               input_5 = 16,
		               input_6 = 32,
		               input_7 = 64,
		               input_8 = 128,
		               error = 1ul << 31;
	}
}