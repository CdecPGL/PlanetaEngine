#pragma once

#include <vector>
#include <memory>
#include <array>
#include "RectAngle.h"
#include "Color.h"

namespace planeta {
	class GraphResource;
	class FontDefinitionResource;
	namespace core {
		class Screen;
	}
	class ScreenDrawerGUI {
	public:
		explicit ScreenDrawerGUI(core::Screen& scr) :screen_(scr) {}
		/*�摜��`��(�`��͈́A�摜��̕`��͈́A���]�t���O�A�摜���\�[�X)*/
		void DrawGraph(const util::RectAngle<int>& draw_area, const util::RectAngle<int>& draw_area_on_graph, bool reverse, const std::shared_ptr<GraphResource>& graph_resource);
		/*���C���[��`��(�_���W�A��(����ł͖���)�A�F)*/
		void DrawWire(const std::vector<Vector2Di>& positions, int width, const planeta::Color& color);
		/*���p�`��`��(�_���W�A�O�p�`�|���S���C���f�b�N�X�A�F)*/
		void DrawPolygon(const std::vector<Vector2Di>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta::Color& color);
		/*�������`��(�`��ʒu�A�g��x�A�`�敶����A�F�A���F�A�t�H���g��`���\�[�X)*/
		void DrawString(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const planeta::Color& color, const planeta::Color& outline_color, const std::shared_ptr<FontDefinitionResource>& font_definition_resource);
	private:
		core::Screen& screen_;
	};
}
