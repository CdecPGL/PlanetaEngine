#pragma once

#include <memory>
#include <vector>
#include <array>
#include "DrawComponent.h"
#include "RectAngle.h"
#include "Vertex2D.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
	}
	namespace components {
		class DrawGraphComponent final : public DrawComponent {
		public:
			DrawGraphComponent();
			~DrawGraphComponent() = default;
			void Draw()override;
			bool SetGraphResource(const std::string& resource_id);
			/*アクセサ*/
			const utility::RectAngle<int>& draw_area()const { return _draw_area; }
			void draw_area(const utility::RectAngle<int>& rect) { _draw_area = rect; _UpdateUVPosition(); }
			const bool reverse()const { return reverse_; }
			void reverse(bool rev) { reverse_ = rev; _UpdateUVPosition(); }
		private:
			/*反転描画フラグ*/
			bool reverse_ = false;
			/*画像上で表示する範囲*/
			utility::RectAngle<int> _draw_area;
			/*画像リソース*/
			std::shared_ptr<resources::GraphResource> _graph_resource;
			/*頂点*/
			std::vector<utility::Vertex2D> _vertexes;
			/*ポリゴンインデックス*/
			std::vector<std::array<int, 3>> _indexes;
			/*ポリゴン情報更新*/
			void _UpdatePolygon();
			/*UV座標更新*/
			void _UpdateUVPosition();
		};
	}
}