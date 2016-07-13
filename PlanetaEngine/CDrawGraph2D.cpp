#include "CDrawGraph2D.h"
#include "ScreenDrawer2D.h"
#include "ResourceManager.h"
#include "RGraph.h"
#include "SystemLog.h"
#include "CTransform2D.h"
#include "IGameObjectForComponent.h"
#include "Matrix2_2.h"
#include "MathConstant.h"
#include "GraphDrawData2D.h"

namespace planeta {
	/*���_��[0]����,[1]�E��,[2]�E��,[3]����Ƃ���*/
	CDrawGraph2D::CDrawGraph2D() :graph_draw_data_(std::make_shared<core::GraphDrawData2D>()) {
		graph_draw_data_->SetVertexCount(4);
		graph_draw_data_->SetPolygonIndexes({ { 0,1,3 } ,{ 1,3,2 } });
	}

	CDrawGraph2D::~CDrawGraph2D() = default;

	bool CDrawGraph2D::SetGraphResource(const std::string& resource_id) {
		auto res = core::ResourceManager::instance().GetResource(resource_id);
		if (res == nullptr) {
			PE_LOG_ERROR("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����", resource_id, ")");
			return false;
		}
		std::shared_ptr<RGraph> gr = std::dynamic_pointer_cast<RGraph>(res);
		if (gr) {
			graph_draw_data_->SetGraphResource(gr);
			_draw_area.Set(Vector2Di(0, 0), Vector2Di(gr->size().x, gr->size().y));
			_UpdateUVPosition();
			return true;
		} else {
			PE_LOG_ERROR("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����", resource_id, "�A�^�C�v��", res->GetType().name(), ")");
			return false;
		}
	}

	void CDrawGraph2D::DrawProc(ScreenDrawer2D& drawer) {
		_UpdatePolygon();
		drawer.DrawGraph(graph_draw_data_);
	}

	void CDrawGraph2D::_UpdatePolygon() {
		//���_�ʒu�̍X�V
		//�`��g��x���l�������c���̒���
		double dwidth((double)_draw_area.width()*GetDrawScale().x), dheight((double)_draw_area.height()*GetDrawScale().y);
		//�Q�[���I�u�W�F�N�g�̌`����Ɖ摜�̕\���ʒu����摜�̕\�����S�ʒu�����߂�
		Vector2Dd center_position = GetDrawCenterPosition();
		//����̒��_�x�N�g��
		Vector2Dd left_up_vertex_vec(dwidth * (-graph_center_.x), dheight * graph_center_.y);
		//�E��̒��_�x�N�g��
		Vector2Dd right_up_vertex_vec(dwidth * (1.0 - graph_center_.x), dheight * graph_center_.y);
		//�����̒��_�x�N�g��
		Vector2Dd left_down_vertex_vec(dwidth * (-graph_center_.x), dheight * (graph_center_.y - 1.0));
		//�E���̒��_�x�N�g��
		Vector2Dd right_down_vertex_vec(dwidth * (1.0 - graph_center_.x), dheight * (graph_center_.y - 1.0));
		//�摜�̉�]�x�ƃQ�[���I�u�W�F�N�g�̉�]�x�A�\�����S�ʒu����e���_�����߂�
		graph_draw_data_->SetVertexPosition(0, static_cast<Vector2Df>(center_position + math::RotationalTransformation(GetDrawRotationRed(), left_down_vertex_vec)));
		graph_draw_data_->SetVertexPosition(1, static_cast<Vector2Df>(center_position + math::RotationalTransformation(GetDrawRotationRed(), right_down_vertex_vec)));
		graph_draw_data_->SetVertexPosition(2, static_cast<Vector2Df>(center_position + math::RotationalTransformation(GetDrawRotationRed(), right_up_vertex_vec)));
		graph_draw_data_->SetVertexPosition(3, static_cast<Vector2Df>(center_position + math::RotationalTransformation(GetDrawRotationRed(), left_up_vertex_vec)));
		//�F
		for (int i = 0; i < 4; ++i) {
			graph_draw_data_->SetVertexColor(i, color());
		}
	}

	void CDrawGraph2D::_UpdateUVPosition() {
		//UV���W�̍X�V
		Vector2Df uvs[4];
		auto g_size = graph_draw_data_->graph_resource()->size();
		//����
		uvs[0].x = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / g_size.x;
		uvs[0].y = (float)(_draw_area.bottom() + 1) / g_size.y;
		//�E��
		uvs[1].x = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / g_size.x;
		uvs[1].y = (float)(_draw_area.bottom() + 1) / g_size.y;
		//�E��
		uvs[2].x = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / g_size.x;
		uvs[2].y = (float)_draw_area.top() / g_size.y;
		//����
		uvs[3].x = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / g_size.x;
		uvs[3].y = (float)_draw_area.top() / g_size.y;

		for (int i = 0; i < 4; ++i) {
			graph_draw_data_->SetVertexUV(i, uvs[i]);
		}
	}
}