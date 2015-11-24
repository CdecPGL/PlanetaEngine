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
	namespace core {
		class GraphDrawData;
	}
	namespace components {
		/*惑星描画コンポーネント。DrawPlanetComponentのseparationはPlanetComponentのseparationとは異なる*/
		class PlanetComponent;
		class DrawPlanetComponent : public DrawComponent {
		public:
			DrawPlanetComponent();
			~DrawPlanetComponent();
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
			/*画像描画データ*/
			std::unique_ptr<core::GraphDrawData> graph_draw_data_;
			virtual bool Initialize_() override;
			virtual void Finalize_() override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*ポリゴンセット*/
			void _SetPolygon();
			/*ポリゴン情報更新*/
			void _UpdatePolygon();
		};
	}
}