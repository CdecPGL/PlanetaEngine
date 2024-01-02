#include "DxLib.h"

#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "standard_rendering_manager.hpp"
#include "screen.hpp"

namespace plnt::private_ {
	bool standard_rendering_manager::initialize() {
		SetDrawScreen(DX_SCREEN_BACK); //バックスクリーンを描画対象に
		SetUseLighting(false); //ライティング計算を行わない
		SetUseSetDrawScreenSettingReset(false); //スクリーン変更時にカメラ情報のリセットを行わない。
		SetDrawMode(DX_DRAWMODE_BILINEAR); //バイリニア法で描画
		return true;
	}

	void standard_rendering_manager::finalize() { }

	bool standard_rendering_manager::update() {
		////デバッグ用
			////グリッド線(32ごとに引く)
		//for (int i = 0; i <= 640; i += 32) {
		//	DrawWire({ Vector2Dd(i,0),Vector2Dd(i,480) }, 1, Color::Green());
		//}
		//for (int i = 0; i <= 480; i += 32) {
		//	DrawWire({ Vector2Dd(0,i),Vector2Dd(640,i) }, 1, Color::Green());
		//}
		//
		//////枠
		//DrawWire({ Vector2Dd(0,0),Vector2Dd(640,0),Vector2Dd(640,480),Vector2Dd(0,480),Vector2Dd(0,0) }, 1,Color::Red());

		////マーカー
			////DrawWire({ Vector2Dd(0,0),Vector2Dd(32,32) }, 1, Color::Blue());
			////DrawWire({ Vector2Dd(640,0),Vector2Dd(608,32) }, 1, Color::Yellow());
			////DrawWire({ Vector2Dd(640,480),Vector2Dd(608,448) }, 1, Color::Magenta());
			////DrawWire({ Vector2Dd(0,480),Vector2Dd(32,448) }, 1, Color::Cyan());

		//メイン画面描画
		SetUseZBuffer3D(true);
		SetWriteZBuffer3D(true);
		//DXライブラリ描画
		main_screen_->handle_draw_reservations();
		//Effekseerの更新
		const auto eff_mgr = GetEffekseer3DManager();
		eff_mgr->Update();
		//Effekseer描画
		const auto eff_renderer = GetEffekseer3DRenderer();
		RenderVertex();
		eff_renderer->BeginRendering();
		eff_mgr->Draw();
		eff_renderer->EndRendering();
		RefreshDxLibDirect3DSetting();

		//GUI画面描画
		SetUseZBuffer3D(false);
		SetWriteZBuffer3D(false);
		gui_screen_->handle_draw_reservations();

		//デバッグ描画
		debug_draw_screen_->handle_draw_reservations();

		//デバッグ情報描画
		debug_information_screen_->handle_draw_reservations();

		//画面更新
		if (!(ScreenFlip() == 0 && ClearDrawScreen() == 0)) { return false; }
		return true;
	}

	std::shared_ptr<screen> standard_rendering_manager::get_main_screen() { return main_screen_; }

	std::shared_ptr<screen> standard_rendering_manager::get_debug_draw_screen() {
		return debug_draw_screen_;
	}

	std::shared_ptr<screen> standard_rendering_manager::get_debug_information_screen() {
		return debug_information_screen_;
	}

	std::shared_ptr<screen> standard_rendering_manager::get_gui_screen() { return gui_screen_; }

	standard_rendering_manager::standard_rendering_manager(): main_screen_(std::make_shared<screen>()),
	                                                      gui_screen_(std::make_shared<screen>()),
	                                                      debug_draw_screen_(std::make_shared<screen>()),
	                                                      debug_information_screen_(std::make_shared<screen>()) { }
}
