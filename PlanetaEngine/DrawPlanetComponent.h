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
		/*�f���`��R���|�[�l���g�BDrawPlanetComponent��separation��PlanetComponent��separation�Ƃ͈قȂ�*/
		class PlanetComponent;
		class DrawPlanetComponent : public DrawComponent {
		public:
			DrawPlanetComponent();
			virtual void Draw() override;
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
			/*�摜���\�[�X*/
			std::shared_ptr<resources::GraphResource> _graph_resource;
			virtual bool Initialize_() override;
			virtual void Finalize_() override;
			utility::WeakPointer<components::PlanetComponent> _planet_component;
			/*���_*/
			std::vector<utility::Vertex2D> _vertexes;
			/*�|���S���C���f�b�N�X*/
			std::vector<std::array<int, 3>> _indexes;
			/*�|���S���Z�b�g*/
			void _SetPolygon();
			/*�|���S�����X�V*/
			void _UpdatePolygon();
		};
	}
}