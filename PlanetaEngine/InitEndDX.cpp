#include "InitEnd.h"
#include "DxLib.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {
		namespace init_end {
			bool InitDxLibrary() {
				ChangeWindowMode(true);
				SetMainWindowText("PlanetaEngine_V0.0.0");
				if (DxLib_Init() < 0) {
					debug::SystemLog::instance().LogError("DX���C�u�����̏������Ɏ��s���܂����B", "InitDxLibrary");
					return false;
				}
				else {
					debug::SystemLog::instance().LogMessage("DX���C�u�����̏������ɐ������܂����B", "InitDxLibrary");
					return true;
				}
			}

			bool EndDxLibrary() {
				debug::SystemLog::instance().LogMessage("DX���C�u�����̏I���������s���܂����B", "EndDxLibrary");
				DxLib_End();
				return true;
			}
		}
	}
}