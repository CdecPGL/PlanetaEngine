#pragma once
#include <vector>
#include <array>
#include <memory>
#include <deque>
#include "PointerSingletonTemplate.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core{
		class Screen;
		class DrawManager final : public utility::PointerSingletonTemplate<DrawManager>{
			friend utility::PointerSingletonTemplate<DrawManager>;
		public:
			bool Initialize() override;
			bool Finalize() override;

			//システム関数
			bool Update();
			//カメラ関数
			/*カメラ位置を設定*/
			void SetCameraPosition(const Vector2D<double>& position);
			/*カメラ回転度を設定*/
			void SetCameraRotationRad(double rotation_rad);
			/*カメラ拡大度を設定*/
			void SetCameraScale(double scale);

			/*新しいスクリーンを作成する(現状は一つのスクリーンにのみ対応)*/
			std::shared_ptr<Screen> CreateScreen();
			/*スクリーンを破棄する*/
			bool DisposeScreen(const std::shared_ptr<Screen>& screen);
		private:
			DrawManager() = default;
			~DrawManager() = default;

			Vector2D<double> camera_posision_;
			double camera_rotation_rad_ = 0.0;
			double camera_scale_ = 1.0;

			std::shared_ptr<Screen> primary_screen_; //メインスクリーン

			bool SetDrawScreen_(const std::shared_ptr<Screen>& screen);
		};
	}
}
