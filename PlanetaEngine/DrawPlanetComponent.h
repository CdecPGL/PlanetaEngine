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
		/*�f���`��R���|�[�l���g�BDrawPlanetComponent��separation��PlanetComponent��separation�Ƃ͈قȂ�*/
		class PlanetComponent;
		class DrawPlanetComponent final: public DrawComponent {
		public:
			DrawPlanetComponent();
			~DrawPlanetComponent();
			bool SetGraphResource(const std::string& resource_id);
			/*�A�N�Z�T*/
			unsigned int horizontal_separation()const { return _horizontal_separation; }
			void horizontal_separation(unsigned int sep);
			unsigned int vertical_separation()const { return _vertical_separation; }
			void vertical_separation(unsigned int sep);
		private:
			/*����������*/
			unsigned int _horizontal_separation;
			/*����������*/
			unsigned int _vertical_separation;
			/*�摜�`��f�[�^*/
			std::unique_ptr<core::GraphDrawData> graph_draw_data_;
			virtual bool OnInitialized() override;
			virtual void OnFinalized() override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*�|���S���Z�b�g*/
			void _SetPolygon();
			/*�|���S�����X�V*/
			void _UpdatePolygon();
			/*�`�揈��*/
			void DrawProc(ScreenDrawer2D& drawer)override;
		};
	}
}