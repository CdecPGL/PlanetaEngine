#include "DxLib.h"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "EngineConfigData.h"
#include "SystemLog.h"
#include "CharacterCode.h"

namespace planeta {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeDxLib() {
				//DXライブラリの初期化
				//////////////////////////////////////////////////////////////////////////
				//ログ出力先を変更
				SetApplicationLogSaveDirectory(system_variables::file_system::LogDirectory.c_str());
				//ウインドウモード設定
				ChangeWindowMode(engine_config::engine::WindowMode());
				//ウインドウタイトル設定(ゲームタイトル+ゲームバージョン)
				SetMainWindowText(std::string(engine_config::game::GameTitle() + " v" + engine_config::game::VersionString()).c_str());
				//描画サイズとカラービット深度設定
				SetGraphMode(engine_config::engine::DrawSize().x, engine_config::engine::DrawSize().y, engine_config::engine::ColorBitDepth());
				//必要ならウインドウサイズに合わせて拡縮
				{
					const auto& ds = engine_config::engine::DrawSize();
					const auto& ws = engine_config::engine::WindowSize();
					if (ds.x != ws.x || ds.y != ws.y) {
						SetWindowSizeExtendRate((double)ws.x / ds.x, (double)ws.y / ds.y);
					}
				}
				if (DxLib_Init() == 0) {
					return{ true,[] {DxLib_End(); } };
				} else { 
					PE_LOG_FATAL("DXライブラリの初期化に失敗しました。");
					return{ false,[] {} };
				}
			}
		}
	}
}