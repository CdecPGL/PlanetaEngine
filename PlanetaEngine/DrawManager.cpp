#include <cassert>
#include "DrawManager.h"
#include "DxLib.h"
#include "DXUtility.h"
#include "MathUtility.h"
#include "Screen.h"
#include "ConfigData.h"

namespace planeta_engine {
	namespace core {
		bool planeta_engine::core::DrawManager::Initialize()
		{
			SetDrawScreen(DX_SCREEN_BACK); //バックスクリーンを描画対象に
			SetupCamera_Ortho((float)config_data::engine::DrawSize().y); //正射影カメラを設定
			SetUseLighting(false); //ライティング計算を行わない
			SetUseSetDrawScreenSettingReset(false); //スクリーン変更時にカメラ情報のリセットを行わない。
			camera_posision_.Set(config_data::engine::DrawSize().x / 2, config_data::engine::DrawSize().y / 2);
			return true;
		}

		bool planeta_engine::core::DrawManager::Finalize()
		{
			return true;
		}

		bool DrawManager::Update()
		{
			//デバッグ用
			//グリッド線(32ごとに引く)
			//for (int i = 0; i <= 640; i += 32) {
			//	DrawWire({ Vector2D<double>(i,0),Vector2D<double>(i,480) }, 1, Color::Green());
			//}
			//for (int i = 0; i <= 480; i += 32) {
			//	DrawWire({ Vector2D<double>(0,i),Vector2D<double>(640,i) }, 1, Color::Green());
			//}
			//
			////枠
			//DrawWire({ Vector2D<double>(0,0),Vector2D<double>(640,0),Vector2D<double>(640,480),Vector2D<double>(0,480),Vector2D<double>(0,0) }, 1,Color::Red());

			//マーカー
			//DrawWire({ Vector2D<double>(0,0),Vector2D<double>(32,32) }, 1, Color::Blue());
			//DrawWire({ Vector2D<double>(640,0),Vector2D<double>(608,32) }, 1, Color::Yellow());
			//DrawWire({ Vector2D<double>(640,480),Vector2D<double>(608,448) }, 1, Color::Magenta());
			//DrawWire({ Vector2D<double>(0,480),Vector2D<double>(32,448) }, 1, Color::Cyan());
			//スクリーンに描画
			primary_screen_->HandleDrawReservations();
			//画面更新
			if (!(ScreenFlip() == 0 && ClearDrawScreen() == 0)) { return false; }
			return true;
		}

		void DrawManager::SetCameraPosition(const Vector2D<double>& position)
		{
			if (!math::DoubleIsEqual(camera_posision_.x, position.x) || !math::DoubleIsEqual(camera_posision_.y, position.y)) {
				camera_posision_ = position;
				SetCameraPositionAndAngle(VGet((float)position.x, (float)position.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());
			}
		}

		void DrawManager::SetCameraRotationRad(double rotation_rad)
		{
			if (!math::DoubleIsEqual(camera_rotation_rad_,rotation_rad)) {
				camera_rotation_rad_ = rotation_rad;
				SetCameraPositionAndAngle(GetCameraPosition(), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)rotation_rad);
			}
		}

		void DrawManager::SetCameraScale(double scale)
		{
			if (!math::DoubleIsEqual(camera_scale_, scale)) {
				camera_scale_ = scale;
				SetupCamera_Ortho(480.0f / (float)scale);
			}
		}

		std::shared_ptr<Screen> DrawManager::CreateScreen() {
			if (primary_screen_) { 
				assert(false); 
				return nullptr;
			}
			primary_screen_ = std::make_shared<Screen>(DX_SCREEN_BACK);
			return primary_screen_;
		}

		bool DrawManager::SetDrawScreen_(const std::shared_ptr<Screen>& screen) {
			int gh = screen->GetHandle();
			SetDrawScreen(gh);
			return true;
		}

		bool DrawManager::DisposeScreen(const std::shared_ptr<Screen>& screen) {
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

