#include "InitEnd.h"
#include "DxLib.h"
#include "SystemLog.h"
#include "SystemVariables.h"

namespace planeta_engine {
	namespace core {
		namespace init_end {
			bool InitDxLibrary() {
				//���O�o�͐��ύX
				SetApplicationLogSaveDirectory(system_variables::LogDirectory.c_str());
				ChangeWindowMode(true);
				SetMainWindowText("PlanetaEngine_V0.0.0");
				if (DxLib_Init() < 0) {
					debug::SystemLog::instance().LogError("DX���C�u�����̏������Ɏ��s���܂����B",__FUNCTION__);
					return false;
				}
				else {
					debug::SystemLog::instance().LogMessage("DX���C�u�����̏������ɐ������܂����B", __FUNCTION__);
					return true;
				}
			}

			bool EndDxLibrary() {
				debug::SystemLog::instance().LogMessage("DX���C�u�����̏I���������s���܂����B", __FUNCTION__);
				DxLib_End();
				return true;
			}
		}
	}
}