#pragma once

#include <array>
#include <vector>
#include "DrawComponent.h"
#include "WeakPointer.h"
#include "Vertex2D.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
	}
	namespace components {
		/*惑星描画コンポーネント。DrawPlanetComponentのseparationはPlanetComponentのseparationとは異なる*/
		class PlanetComponent;
		class DrawPlanetComponent : public DrawComponent {
		public:
			DrawPlanetComponent();
			virtual void Draw() override;
			bool SetGraphResource(const std::string& resource_id);
			/*アクセサ*/
			unsigned int horizontal_separation()const { return _horizontal_separation; }
			void horizontal_separation(unsigned int sep);
			unsigned int vertical_separation()const { return _vertical_separation; }
			void vertical_separation(unsigned int sep);
		private:
			/*水平分割数*/
			unsigned int _horizontal_separation;
			/*垂直分割数*/
			unsigned int _vertical_separation;
			/*画像リソース*/
			std::shared_ptr<resources::GraphResource> _graph_resource;
			virtual bool Initialize_() override;
			virtual void Finalize_() override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*頂点*/
			std::vector<utility::Vertex2D> _vertexes;
			/*ポリゴンインデックス*/
			std::vector<std::array<int, 3>> _indexes;
			/*ポリゴンセット*/
			void _SetPolygon();
			/*ポリゴン情報更新*/
			void _UpdatePolygon();
		};
	}
}