#pragma once

#include <vector>
#include <array>
#include "DrawComponent.h"

namespace planeta_engine {
	namespace components {
		class DrawPolygonComponent final: public DrawComponent{
		public:
			void Draw()override;
			void SetNumberOfVertexes(size_t n);
			/*�w��C���f�b�N�X�̍��W��ݒ�*/
			bool SetVertexPosition(size_t idx, const math::Vector2Dd& pos);
			/*���_��ݒ�*/
			void SetVertexes(const std::vector<math::Vector2Dd>& vs);
		private:
			std::vector<math::Vector2Dd> points_;
			std::vector<math::Vector2Dd> vertexes_;
			std::vector<std::array<int, 3>> indexes_;
			void UpdateVertex_();
			void UpdatePorygonIndex_();
		};
	}
}