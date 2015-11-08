#include "InitEnd.h"
#include "DxLib.h"
#include "SystemLog.h"
#include "SystemVariables.h"

namespace planeta_engine {
	namespace core {
		namespace init_end {
			bool InitDxLibrary() {
				//ログ出力先を変更
				SetApplicationLogSaveDirectory(system_variables::LogDirectory.c_str());
				ChangeWindowMode(true);
				SetMainWindowText("PlanetaEngine_V0.0.0");
				if (DxLib_Init() < 0) {
					debug::SystemLog::instance().LogError("DXライブラリの初期化に失敗しました。",__FUNCTION__);
					return false;
				}
				else {
					debug::SystemLog::instance().LogMessage("DXライブラリの初期化に成功しました。", __FUNCTION__);
					return true;
				}
			}

			bool EndDxLibrary() {
				debug::SystemLog::instance().LogMessage("DXライブラリの終了処理を行いました。", __FUNCTION__);
				DxLib_End();
				return true;
			}
		}
	}
}