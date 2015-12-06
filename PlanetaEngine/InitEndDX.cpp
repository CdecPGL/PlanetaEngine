#include "InitEnd.h"
#include <string>
#include "DxLib.h"
#include "SystemLog.h"
#include "SystemVariables.h"
#include "ConfigData.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core {
		namespace init_end {
			bool InitDxLibrary() {
				//ログ出力先を変更
				SetApplicationLogSaveDirectory(system_variables::LogDirectory.c_str());
				//ウインドウモード設定
				ChangeWindowMode(config_data::engine::WindowMode());
				//ウインドウタイトル設定(ゲームタイトル+ゲームバージョン)
				SetMainWindowText((config_data::game::GameTitle() + " v" + config_data::game::VersionString()).c_str());
				//描画サイズとカラービット深度設定
				SetGraphMode(config_data::engine::DrawSize().x, config_data::engine::DrawSize().y, config_data::engine::ColorBitDepth());
				//必要ならウインドウサイズに合わせて拡縮
				{
					const auto& ds = config_data::engine::DrawSize();
					const auto& ws = config_data::engine::WindowSize();
					if (ds.x != ws.x || ds.y != ws.y) {
						SetWindowSizeExtendRate((double)ws.x / ds.x, (double)ws.y / ds.y);
					}
				}
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