#include <windows.h>

#include "windows_utility.hpp"

namespace plnt::win::console {
	void set_character_color(const int col) {
		const HANDLE h_cons = GetStdHandle(STD_OUTPUT_HANDLE); // コンソールハンドル取得
		WORD attr = 0; //  API に渡すアトリビュート
		if (col & col_intensity) // 高輝度ビットが立っていれば
			attr |= FOREGROUND_INTENSITY; //  アトリビュートの高輝度ビットを立てる
		if (col & col_red_mask)
			attr |= FOREGROUND_RED;
		if (col & col_green_mask)
			attr |= FOREGROUND_GREEN;
		if (col & col_blue_mask)
			attr |= FOREGROUND_BLUE;
		SetConsoleTextAttribute(h_cons, attr); // 色指定
	}
}
