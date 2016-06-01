#pragma once

#include <memory>
#include <vector>
#include <array>
#include "Vector2D.h"
#include "Color.h"

namespace planeta {
	namespace core {
		class Screen;
		class GraphDrawData2D;
	}
	class ScreenDrawer2D {
	public:
		explicit ScreenDrawer2D(core::Screen& scr) :screen_(scr) {}
		/*�摜�̕`��(�摜�`��f�[�^)*/
		void DrawGraph(const std::shared_ptr<core::GraphDrawData2D>& graph_draw_data);
		/*���C���[�̕`��(�_���W�A��(����ł͖���)�A�F)*/
		void DrawWire(const std::vector<Vector2Dd>& positions, double width, const planeta::Color& color);
		/*���p�`�̕`��(�_���W�A�O�p�`�|���S���C���f�b�N�X�A�F)*/
		void DrawPolygon(const std::vector<Vector2Dd>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta::Color& color);
	private:
		core::Screen& screen_;
	};
}
