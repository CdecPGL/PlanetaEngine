#include "DxLib.h"

#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/effekseer/EffekseerUtil.hpp"

#include "InitFunctions.hpp"
#include "SystemVariables.hpp"
#include "LogUtility.hpp"
#include "CharacterCode.hpp"
#include "ConfigManager.hpp"

namespace plnt {
	namespace private_ {
		namespace init_funcs {
			//////////////////////////////////////////////////////////////////////////
			//DXライブラリの初期化
			//////////////////////////////////////////////////////////////////////////
			std::tuple<bool, std::function<void()>> InitializeDxLib(ConfigManager& cfg_mgr) {
				//ログ出力先を変更
				SetApplicationLogSaveDirectory(system_variables::file_system::LogDirectory.c_str());
				//ウインドウモード設定
				ChangeWindowMode(cfg_mgr.is_window_mode());
				//ウインドウタイトル設定(ゲームタイトル+ゲームバージョン)
				SetMainWindowText(std::string(cfg_mgr.game_title() + " v" + cfg_mgr.game_version_string()).c_str());
				//描画サイズとカラービット深度設定
				SetGraphMode(cfg_mgr.draw_size().x, cfg_mgr.draw_size().y, cfg_mgr.color_bit_depth());
				//必要ならウインドウサイズに合わせて拡縮
				{
					const auto& ds = cfg_mgr.draw_size();
					const auto& ws = cfg_mgr.window_size();
					if (ds.x != ws.x || ds.y != ws.y) {
						SetWindowSizeExtendRate((double)ws.x / ds.x, (double)ws.y / ds.y);
					}
				}
				//Effekseerを利用するために、DirextX11を使用するよう設定する
				SetUseDirect3DVersion(DX_DIRECT3D_11);
				//DXライブラリの初期化
				if (DxLib_Init() < 0) { 
					PE_LOG_FATAL("DXライブラリの初期化に失敗しました。");
					return{ false,[] {} };
				}
				//マウスカーソルの表示を設定
				SetMouseDispFlag(cfg_mgr.is_cursor_visible());
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
				eff_mgr->SetTextureLoader(::Effekseer::MakeRefPtr<effekseer::TextureLoaderForEffekseer>());

				return{ true,[] { Effkseer_End(); } };
			}
		}
	}
}