#pragma once

namespace plnt::win::console {
	constexpr int col_black = 0x00;
	constexpr int col_dark_blue = 0x01;
	constexpr int col_dark_green = 0x02;
	constexpr int col_dark_cyan = 0x03;
	constexpr int col_dark_red = 0x04;
	constexpr int col_dark_violet = 0x05;
	constexpr int col_dark_yellow = 0x06;
	constexpr int col_gray = 0x07;
	constexpr int col_light_gray = 0x08;
	constexpr int col_blue = 0x09;
	constexpr int col_green = 0x0a;
	constexpr int col_cyan = 0x0b;
	constexpr int col_red = 0x0c;
	constexpr int col_violet = 0x0d;
	constexpr int col_yellow = 0x0e;
	constexpr int col_white = 0x0f;
	constexpr int col_intensity = 0x08; // 高輝度マスク
	constexpr int col_red_mask = 0x04; // 赤色ビット
	constexpr int col_green_mask = 0x02; // 緑色ビット
	constexpr int col_blue_mask = 0x01; //  青色ビット
	void set_character_color(int col);
}
