#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "EngineConfigData.h"
#include "LogUtility.h"
#include "CharacterCode.h"

#include "EffekseerUtil.h"

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			//////////////////////////////////////////////////////////////////////////
			//DXライブラリの初期化
			//////////////////////////////////////////////////////////////////////////
			std::tuple<bool, std::function<void()>> InitializeDxLib() {
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
				//Effekseerを利用するために、DirextX9を使用するよう設定する
				SetUseDirect3DVersion(DX_DIRECT3D_9);
				//DXライブラリの初期化
				if (DxLib_Init() < 0) { 
					PE_LOG_FATAL("DXライブラリの初期化に失敗しました。");
					return{ false,[] {} };
				}
				return{ true,[] { DxLib_End(); } };
			}
			//////////////////////////////////////////////////////////////////////////
			//Effekseerの初期化
			//////////////////////////////////////////////////////////////////////////
			class 
			std::tuple<bool, std::function<void()>> InitializeEffekseer() {
				//Effekseerの初期化
				if (Effkseer_Init(2000) < 0) {
					PE_LOG_FATAL("Effekseerの初期化に失敗しました。");
					return{ false,[] {} };
				}
				if (Effekseer_InitDistortion(1) < 0) {
					PE_LOG_WARNING("Effekseerの歪み機能を初期化できませんでした。");
				}
				//Effeseerの設定
				SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
				Effekseer_SetGraphicsDeviceLostCallbackFunctions();
				SetUseZBuffer3D(true);
				SetWriteZBuffer3D(true);
				decltype(auto) eff_mgr = GetEffekseer3DManager();
				//eff_mgr->SetEffectLoader(new private_::EffectLoaderForEffekseer{});
				eff_mgr->SetTextureLoader(new private_::TextureLoaderForEffekseer{});

				return{ true,[] { Effkseer_End(); } };
			}
		}
	}
}