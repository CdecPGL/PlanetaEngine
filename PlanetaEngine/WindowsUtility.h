#pragma once

namespace planeta_engine {
	namespace windows {
		namespace console {
			constexpr int COL_BLACK = 0x00;
			constexpr int COL_DARK_BLUE = 0x01;
			constexpr int COL_DARK_GREEN = 0x02;
			constexpr int COL_DARK_CYAN = 0x03;
			constexpr int COL_DARK_RED = 0x04;
			constexpr int COL_DARK_VIOLET = 0x05;
			constexpr int COL_DARK_YELLOW = 0x06;
			constexpr int COL_GRAY = 0x07;
			constexpr int COL_LIGHT_GRAY = 0x08;
			constexpr int COL_BLUE = 0x09;
			constexpr int COL_GREEN = 0x0a;
			constexpr int COL_CYAN = 0x0b;
			constexpr int COL_RED = 0x0c;
			constexpr int COL_VIOLET = 0x0d;
			constexpr int COL_YELLOW = 0x0e;
			constexpr int COL_WHITE = 0x0f;
			constexpr int COL_INTENSITY = 0x08;		// 高輝度マスク
			constexpr int COL_RED_MASK = 0x04;		// 赤色ビット
			constexpr int COL_GREEN_MASK = 0x02;	// 緑色ビット
			constexpr int COL_BLUE_MASK = 0x01;		//  青色ビット
			void SetCharacterColor(int col);
		}
	}
}