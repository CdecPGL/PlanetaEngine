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
		class GraphDrawData2D;
	}
	namespace components {
		/*惑星描画コンポーネント。DrawPlanetComponentのseparationはPlanetComponentのseparationとは異なる*/
		class PlanetComponent;
		class DrawPlanetComponent final: public DrawComponent {
		public:
			DrawPlanetComponent();
			~DrawPlanetComponent();
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
			std::shared_ptr<core::GraphDrawData2D> graph_draw_data_;
			virtual bool OnInitialized() override;
			virtual void OnFinalized() override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*ポリゴンセット*/
			void _SetPolygon();
			/*ポリゴン情報更新*/
			void _UpdatePolygon();
			/*描画処理*/
			void DrawProc(ScreenDrawer2D& drawer)override;
		};
	}
}