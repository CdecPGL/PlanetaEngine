#include <windows.h>
#include "planeta/win/WindowsUtility.hpp"

namespace plnt {
	namespace windows {
		namespace console {
			void SetCharacterColor(int col) {
				HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);     // コンソールハンドル取得
				WORD attr = 0;     //  API に渡すアトリビュート
				if (col & COL_INTENSITY)       // 高輝度ビットが立っていれば
					attr |= FOREGROUND_INTENSITY;      //  アトリビュートの高輝度ビットを立てる
				if (col & COL_RED_MASK)
					attr |= FOREGROUND_RED;
				if (col & COL_GREEN_MASK)
					attr |= FOREGROUND_GREEN;
				if (col & COL_BLUE_MASK)
					attr |= FOREGROUND_BLUE;
				SetConsoleTextAttribute(hCons, attr);        // 色指定
			}
		}
	}
}