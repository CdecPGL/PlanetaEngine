#pragma once

#include <memory>
#include <vector>
#include <array>
#include "Vector2D.h"
#include "Color.h"

namespace planeta_engine {
	namespace core {
		class Screen;
		class GraphDrawData;
		class ScreenDrawer2D {
		public:
			explicit ScreenDrawer2D(Screen& scr) :screen_(scr) {}
			/*�摜�̕`��(�摜�`��f�[�^)*/
			void DrawGraph(const GraphDrawData& graph_draw_data);
			/*���C���[�̕`��(�_���W�A��(����ł͖���)�A�F)*/
			void DrawWire(const std::vector<Vector2D<double>>& positions, double width, const Color& color);
			/*���p�`�̕`��(�_���W�A�O�p�`�|���S���C���f�b�N�X�A�F)*/
			void DrawPolygon(const std::vector<Vector2D<double>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color);
		private:
			Screen& screen_;
		};
	}
}
