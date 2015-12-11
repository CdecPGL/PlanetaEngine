#include <cassert>
#include "DrawManager.h"
#include "DxLib.h"
#include "DXUtility.h"
#include "GraphResource.h"
#include "FontDefinitionResource.h"
#include "MathUtility.h"
#include "ScreenChangeEffecter.h"
#include "Screen.h"
#include "GraphDrawData.h"
#include "DXGraphDrawData.h"
#include "ConfigData.h"

namespace planeta_engine {
	namespace core {
		bool planeta_engine::core::DrawManager::Initialize()
		{
			SetDrawScreen(DX_SCREEN_BACK); //バックスクリーンを描画対象に
			SetupCamera_Ortho(config_data::engine::DrawSize().y); //正射影カメラを設定
			SetUseLighting(false); //ライティング計算を行わない
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

		void DrawManager::DrawUIGraph(const utility::RectAngle<int>& draw_area, const utility::RectAngle<int>& draw_area_on_graph,bool reverse, const std::shared_ptr<resources::GraphResource>& graph_resource)
		{
			if (graph_resource == nullptr) { return; }
			double scale_x = (double)draw_area.size.x / draw_area_on_graph.size.x;
			double scale_y = (double)draw_area.size.y / draw_area_on_graph.size.y;
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawRectRotaGraph3(draw_area.position.x, draw_area.position.y, draw_area_on_graph.position.x, draw_area_on_graph.position.y, draw_area.size.x, draw_area.size.y, draw_area_on_graph.size.x / 2, draw_area_on_graph.size.y / 2, scale_x, scale_y, 0.0, graph_resource->GetHandle(), true, reverse);
		}

		void DrawManager::DrawUIWire(const std::vector<Vector2D<int>>& positions, int width, const Color& color)
		{
			using namespace utility::dx;
			int dxc = PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < (int)positions.size() - 1; ++i) {
				DrawLine(positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y, dxc, width);
			}
		}

		void DrawManager::DrawUIPolygon(const std::vector<Vector2D<int>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color)
		{
			using namespace utility::dx;
			int dxc = PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (size_t i = 0; i < indexes.size(); ++i) {
					const auto& v0 = positions.at(indexes[i][0]);
					const auto& v1 = positions.at(indexes[i][1]);
					const auto& v2 = positions.at(indexes[i][2]);
					DrawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, dxc, true);
				}
			}
			catch (std::out_of_range&) {
				//インデックス番号が範囲外を指していた
			}
		}

		void DrawManager::DrawUIString(const Vector2D<int>& position, const Vector2D<double> scale, const std::string& str, const planeta_engine::Color& color, const planeta_engine::Color& outline_color, const std::shared_ptr<resources::FontDefinitionResource>& font_definition_resource)
		{
			if (font_definition_resource == nullptr) { return; }
			using namespace utility::dx;
			DrawExtendStringToHandle(position.x, position.y, scale.x, scale.y, str.c_str(), PEColorToDXColorHandle(color), font_definition_resource->GetHandle(), PEColorToDXColorHandle(outline_color));
		}

		std::shared_ptr<Screen> DrawManager::CreateScreen() {
			if (primary_screen_) { 
				assert(false); 
				return nullptr;
			}
			primary_screen_ = std::make_shared<Screen>(DX_SCREEN_BACK, true);
			return primary_screen_;
		}

		bool DrawManager::SetDrawScreen_(const std::shared_ptr<Screen>& screen) {
			int gh = screen->GetHandle();
			SetDrawScreen(gh);
			return true;
		}

	}
}

