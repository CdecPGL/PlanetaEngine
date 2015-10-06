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
			/*�A�N�Z�T*/
			const utility::RectAngle<int>& draw_area()const { return _draw_area; }
			void draw_area(const utility::RectAngle<int>& rect) { _draw_area = rect; _UpdateUVPosition(); }
			const bool reverse()const { return reverse_; }
			void reverse(bool rev) { reverse_ = rev; _UpdateUVPosition(); }
		private:
			/*���]�`��t���O*/
			bool reverse_ = false;
			/*�摜��ŕ\������͈�*/
			utility::RectAngle<int> _draw_area;
			/*�摜���\�[�X*/
			std::shared_ptr<resources::GraphResource> _graph_resource;
			/*���_*/
			std::vector<utility::Vertex2D> _vertexes;
			/*�|���S���C���f�b�N�X*/
			std::vector<std::array<int, 3>> _indexes;
			/*�|���S�����X�V*/
			void _UpdatePolygon();
			/*UV���W�X�V*/
			void _UpdateUVPosition();
		};
	}
}