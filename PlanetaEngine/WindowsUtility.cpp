#include <windows.h>
#include "WindowsUtility.h"

namespace planeta_engine {
	namespace windows {
		namespace console {
			void SetCharacterColor(int col) {
				HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);     // �R���\�[���n���h���擾
				WORD attr = 0;     //  API �ɓn���A�g���r���[�g
				if (col & COL_INTENSITY)       // ���P�x�r�b�g�������Ă����
					attr |= FOREGROUND_INTENSITY;      //  �A�g���r���[�g�̍��P�x�r�b�g�𗧂Ă�
				if (col & COL_RED_MASK)
					attr |= FOREGROUND_RED;
				if (col & COL_GREEN_MASK)
					attr |= FOREGROUND_GREEN;
				if (col & COL_BLUE_MASK)
					attr |= FOREGROUND_BLUE;
				SetConsoleTextAttribute(hCons, attr);        // �F�w��
			}
		}
	}
}