#pragma once
#include <vector>
#include <array>
#include <memory>
#include <deque>
#include "PointerSingletonTemplate.h"
#include "Vector2D.h"
#include "Color.h"
#include "RectAngle.h"
#include "Vertex2D.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
		class FontDefinitionResource;
	}
	namespace core {
		class ScreenChangeEffecter;
		class GraphDrawData;
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

			//UI描画関数
			/*UI画像を描画(描画範囲、画像上の描画範囲、反転フラグ、画像リソース)*/
			void DrawUIGraph(const utility::RectAngle<int>& draw_area, const utility::RectAngle<int>& draw_area_on_graph,bool reverse, const std::shared_ptr<resources::GraphResource>& graph_resource);
			/*UIワイヤーを描画(点座標、幅(現状では無効)、色)*/
			void DrawUIWire(const std::vector<Vector2D<int>>& positions, int width, const Color& color);
			/*UI多角形を描画(点座標、三角形ポリゴンインデックス、色)*/
			void DrawUIPolygon(const std::vector<Vector2D<int>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color);
			/*文字列を描画(描画位置、拡大度、描画文字列、色、縁色、フォント定義リソース)*/
			void DrawUIString(const Vector2D<int>& position, const Vector2D<double> scale, const std::string& str, const planeta_engine::Color& color, const planeta_engine::Color& outline_color, const std::shared_ptr<resources::FontDefinitionResource>& font_definition_resource);

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
