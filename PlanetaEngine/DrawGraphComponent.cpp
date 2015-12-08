#include "DrawGraphComponent.h"
#include "DrawManager.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "SystemLog.h"
#include "TransformComponent.h"
#include "IGameObjectAccessor.h"
#include "Matrix.h"
#include "MathConstant.h"
#include "GraphDrawData.h"

namespace planeta_engine {
	namespace components {
		/*頂点は[0]左下,[1]右下,[2]右上,[3]左上とする*/
		DrawGraphComponent::DrawGraphComponent() :graph_draw_data_(std::make_unique<core::GraphDrawData>())
		{
			graph_draw_data_->SetVertexCount(4);
			graph_draw_data_->SetPolygonIndexes({ { 0,1,3 } ,{ 1,3,2 } });
		}

		DrawGraphComponent::~DrawGraphComponent() = default;

		bool DrawGraphComponent::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("リソースの取得に失敗しました。(リソース名は") + resource_id + ")", "DrawGraphComponent::SetGraphResource");
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				graph_draw_data_->SetGraphResource(gr);
				_draw_area.Set(math::Vector2Di(0, 0), math::Vector2Di(gr->size().x, gr->size().y));
				_UpdateUVPosition();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("画像リソースでないリソースが指定されました。(リソース名は") + resource_id + "、タイプは" + res->GetType().name() + ")", "DrawGraphComponent::SetGraphResource");
				return false;
			}
		}

		void DrawGraphComponent::Draw()
		{
			_UpdatePolygon();
			core::DrawManager::instance().DrawGraph(*graph_draw_data_);
		}

		void DrawGraphComponent::_UpdatePolygon()
		{
			//頂点位置の更新
			//描画拡大度を考慮した縦横の長さ
			double dwidth((double)_draw_area.width()*GetDrawScale().x), dheight((double)_draw_area.height()*GetDrawScale().y);
			//ゲームオブジェクトの形状情報と画像の表示位置から画像の表示中心位置を求める
			math::Vector2Dd center_position = GetDrawCenterPosition();
			//左下の頂点ベクトル
			math::Vector2Dd left_down_vertex_vec(-dwidth / 2.0, -dheight / 2.0);
			//右下の頂点ベクトル
			math::Vector2Dd right_down_vertex_vec(dwidth / 2.0, -dheight / 2.0);
			//画像の回転度とゲームオブジェクトの回転度、表示中心位置から各頂点を求める
			graph_draw_data_->SetVertexPosition(0, center_position + math::RotationalTransformation(GetDrawRotationRed(), left_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(1, center_position + math::RotationalTransformation(GetDrawRotationRed(), right_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(2, center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, left_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(3, center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, right_down_vertex_vec));
		}

		void DrawGraphComponent::_UpdateUVPosition()
		{
			//UV座標の更新
			math::Vector2Df uvs[4];
			auto g_size = graph_draw_data_->graph_resource()->size();
			//左下
			uvs[0].x = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / g_size.x;
			uvs[0].y = (float)(_draw_area.bottom() + 1) / g_size.y;
			//右下
			uvs[1].x = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / g_size.x;
			uvs[1].y = (float)(_draw_area.bottom() + 1) / g_size.y;
			//右上
			uvs[2].x = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / g_size.x;
			uvs[2].y = (float)_draw_area.top() / g_size.y;
			//左上
			uvs[3].x = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / g_size.x;
			uvs[3].y = (float)_draw_area.top() / g_size.y;

			for (int i = 0; i < 4; ++i) {
				graph_draw_data_->SetVertexUV(i, uvs[i]);
			}
		}
	}
}