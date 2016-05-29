#include <cassert>
#include "RenderManager.h"
#include "DxLib.h"
#include "Screen.h"

namespace planeta_engine {
	namespace core {
		bool planeta_engine::core::RenderManager::Initialize()
		{
			SetDrawScreen(DX_SCREEN_BACK); //バックスクリーンを描画対象に
			SetUseLighting(false); //ライティング計算を行わない
			SetUseSetDrawScreenSettingReset(false); //スクリーン変更時にカメラ情報のリセットを行わない。
			return true;
		}

		void planeta_engine::core::RenderManager::Finalize()
		{
		}

		bool RenderManager::Update()
		{
			//スクリーンに描画
			primary_screen_->HandleDrawReservations();
			//画面更新
			if (!(ScreenFlip() == 0 && ClearDrawScreen() == 0)) { return false; }
			return true;
		}

		std::shared_ptr<Screen> RenderManager::CreateScreen() {
			if (primary_screen_) { 
				assert(false); 
				return nullptr;
			}
			primary_screen_ = std::make_shared<Screen>(DX_SCREEN_BACK);
			return primary_screen_;
		}

		bool RenderManager::SetDrawScreen_(const std::shared_ptr<Screen>& screen) {
			int gh = screen->GetHandle();
			SetDrawScreen(gh);
			return true;
		}

		bool RenderManager::DisposeScreen(const std::shared_ptr<Screen>& screen) {
			if (primary_screen_ && primary_screen_.get() == screen.get()) {
				primary_screen_.reset();
				return true;
			} else {
				assert(false);
				return true;
			}
		}

	}
}

