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
			enum class TextureMappingMode { Round, Plain };
			/*アクセサ*/
			unsigned int horizontal_separation()const { return _horizontal_separation; }
			DrawPlanetComponent& horizontal_separation(unsigned int sep);
			unsigned int vertical_separation()const { return _vertical_separation; }
			DrawPlanetComponent& vertical_separation(unsigned int sep);
			TextureMappingMode texture_mapping_mode()const { return tex_map_mode_; }
			void texture_mapping_mode(TextureMappingMode tmm);
		private:
			/*水平分割数*/
			unsigned int _horizontal_separation;
			/*垂直分割数*/
			unsigned int _vertical_separation;
			/*ポリゴンの設定が必要か*/
			bool set_polygon_flag_ = false;
			/*画像描画データ*/
			std::shared_ptr<core::GraphDrawData2D> graph_draw_data_;
			/*テクスチャマッピングモード*/
			TextureMappingMode tex_map_mode_;
			virtual bool OnInitialized() override;
			virtual void OnFinalized()noexcept override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*ポリゴンセット*/
			void(DrawPlanetComponent::*polygon_setter_)();
			void SetPolygon_();
			void SetPolygonRoundly_();
			void SetPolygonPlainly_();
			/*ポリゴン情報更新*/
			void(DrawPlanetComponent::*polygon_updater_)();
			void UpdatePolygon_();
			void UpdatePolygonRoundly_();
			void UpdatePolygonPlainly_();
			/*描画処理*/
			void DrawProc(ScreenDrawer2D& drawer)override;
		};
	}
}