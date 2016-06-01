#pragma once

#include <memory>
#include <vector>
#include <array>
#include "CDraw2D.h"
#include "RectAngle.h"
#include "Vertex2D.h"

namespace planeta {
	namespace core {
		class GraphDrawData2D;
	}
	namespace resources {
		class GraphResource;
	}
	class CDrawGraph2D final : public CDraw2D {
	public:
		CDrawGraph2D();
		~CDrawGraph2D();
		bool SetGraphResource(const std::string& resource_id);
		/*�A�N�Z�T*/
		const util::RectAngle<int>& draw_area()const { return _draw_area; }
		CDrawGraph2D& draw_area(const util::RectAngle<int>& rect) { _draw_area = rect; _UpdateUVPosition(); return *this; }
		const bool reverse()const { return reverse_; }
		CDrawGraph2D& reverse(bool rev) { reverse_ = rev; _UpdateUVPosition(); return *this; }
		const Vector2Dd& graph_center()const { return graph_center_; }
		void graph_center(const Vector2Dd& pos) { graph_center_ = pos; }
	private:
		/*���]�`��t���O*/
		bool reverse_ = false;
		/*�摜��ŕ\������͈�*/
		util::RectAngle<int> _draw_area;
		/*�摜�̒��S�ʒu*/
		Vector2Dd graph_center_ = Vector2Dd(0.5, 0.5);
		/*�摜�`��f�[�^*/
		std::shared_ptr<core::GraphDrawData2D> graph_draw_data_;
		/*�|���S�����X�V*/
		void _UpdatePolygon();
		/*UV���W�X�V*/
		void _UpdateUVPosition();
		/*�`�揈��*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CDrawGraph2D);
}