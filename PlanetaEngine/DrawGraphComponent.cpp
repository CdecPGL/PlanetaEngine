#include "DrawGraphComponent.h"
#include "DrawManager.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "SystemLog.h"
#include "TransformComponent.h"
#include "IGameObjectAccessor.h"
#include "Matrix.h"
#include "MathConstant.h"
#include "GraphDrawData.h"

namespace planeta_engine {
	namespace components {
		/*���_��[0]����,[1]�E��,[2]�E��,[3]����Ƃ���*/
		DrawGraphComponent::DrawGraphComponent() :graph_draw_data_(std::make_unique<core::GraphDrawData>())
		{
			graph_draw_data_->SetVertexCount(4);
			graph_draw_data_->SetPolygonIndexes({ { 0,1,3 } ,{ 1,3,2 } });
		}

		DrawGraphComponent::~DrawGraphComponent() = default;

		bool DrawGraphComponent::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����") + resource_id + ")", "DrawGraphComponent::SetGraphResource");
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				graph_draw_data_->SetGraphResource(gr);
				_draw_area.Set(math::Vector2Di(0, 0), math::Vector2Di(gr->size().x, gr->size().y));
				_UpdateUVPosition();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����") + resource_id + "�A�^�C�v��" + res->GetType().name() + ")", "DrawGraphComponent::SetGraphResource");
				return false;
			}
		}

		void DrawGraphComponent::Draw()
		{
			_UpdatePolygon();
			core::DrawManager::instance().DrawGraph(*graph_draw_data_);
		}

		void DrawGraphComponent::_UpdatePolygon()
		{
			//���_�ʒu�̍X�V
			//�`��g��x���l�������c���̒���
			double dwidth((double)_draw_area.width()*GetDrawScale().x), dheight((double)_draw_area.height()*GetDrawScale().y);
			//�Q�[���I�u�W�F�N�g�̌`����Ɖ摜�̕\���ʒu����摜�̕\�����S�ʒu�����߂�
			math::Vector2Dd center_position = GetDrawCenterPosition();
			//�����̒��_�x�N�g��
			math::Vector2Dd left_down_vertex_vec(-dwidth / 2.0, -dheight / 2.0);
			//�E���̒��_�x�N�g��
			math::Vector2Dd right_down_vertex_vec(dwidth / 2.0, -dheight / 2.0);
			//�摜�̉�]�x�ƃQ�[���I�u�W�F�N�g�̉�]�x�A�\�����S�ʒu����e���_�����߂�
			graph_draw_data_->SetVertexPosition(0, center_position + math::RotationalTransformation(GetDrawRotationRed(), left_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(1, center_position + math::RotationalTransformation(GetDrawRotationRed(), right_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(2, center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, left_down_vertex_vec));
			graph_draw_data_->SetVertexPosition(3, center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, right_down_vertex_vec));
		}

		void DrawGraphComponent::_UpdateUVPosition()
		{
			//UV���W�̍X�V
			math::Vector2Df uvs[4];
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
}