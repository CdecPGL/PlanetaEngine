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
					debug::SystemLog::instance().LogError("DXライブラリの初期化に失敗しました。", "InitDxLibrary");
					return false;
				}
				else {
					debug::SystemLog::instance().LogMessage("DXライブラリの初期化に成功しました。", "InitDxLibrary");
					return true;
				}
			}

			bool EndDxLibrary() {
				debug::SystemLog::instance().LogMessage("DXライブラリの終了処理を行いました。", "EndDxLibrary");
				DxLib_End();
				return true;
			}
		}
	}
}