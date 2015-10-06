#include "DrawGraphComponent.h"
#include "DrawManager.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "SystemLog.h"
#include "TransformComponent.h"
#include "IGameObjectAccessor.h"
#include "Matrix.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {
		/*���_��[0]����,[1]�E��,[2]�E��,[3]����Ƃ���*/
		DrawGraphComponent::DrawGraphComponent() :_vertexes(4), _indexes({ {0,1,3} ,{1,3,2} })
		{

		}

		bool DrawGraphComponent::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����") + resource_id + ")", "DrawGraphComponent::SetGraphResource");
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				_graph_resource = gr;
				_draw_area.Set(Vector2D<int>(0, 0), Vector2D<int>(gr->size().x, gr->size().y));
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
			core::DrawManager::instance().DrawGraph(_vertexes, _indexes, _graph_resource);
		}

		void DrawGraphComponent::_UpdatePolygon()
		{
			if (game_object() == nullptr || _graph_resource == nullptr) { return; }
			TransformComponent& transform = *(game_object()->GetTransformComponent());
			//���_�ʒu�̍X�V
			//�`��g��x���l�������c���̒���
			double dwidth((double)_draw_area.width()*GetDrawScale().x), dheight((double)_draw_area.height()*GetDrawScale().y);
			//�Q�[���I�u�W�F�N�g�̌`����Ɖ摜�̕\���ʒu����摜�̕\�����S�ʒu�����߂�
			Vector2D<double> center_position = GetDrawCenterPosition();
			//�����̒��_�x�N�g��
			Vector2D<double> left_down_vertex_vec(-dwidth / 2.0, -dheight / 2.0);
			//�E���̒��_�x�N�g��
			Vector2D<double> right_down_vertex_vec(dwidth / 2.0, -dheight / 2.0);
			//�摜�̉�]�x�ƃQ�[���I�u�W�F�N�g�̉�]�x�A�\�����S�ʒu����e���_�����߂�
			_vertexes[0].position = center_position + math::RotationalTransformation(GetDrawRotationRed(), left_down_vertex_vec);
			_vertexes[1].position = center_position + math::RotationalTransformation(GetDrawRotationRed(), right_down_vertex_vec);
			_vertexes[2].position = center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, left_down_vertex_vec);
			_vertexes[3].position = center_position + math::RotationalTransformation(GetDrawRotationRed() + math::constant::PI, right_down_vertex_vec);
		}

		void DrawGraphComponent::_UpdateUVPosition()
		{
			//UV���W�̍X�V
			//����
			_vertexes[0].u = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / _graph_resource->size().x;
			_vertexes[0].v = (float)(_draw_area.bottom() + 1) / _graph_resource->size().y;
			//�E��
			_vertexes[1].u = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / _graph_resource->size().x;
			_vertexes[1].v = (float)(_draw_area.bottom() + 1) / _graph_resource->size().y;
			//�E��
			_vertexes[2].u = (float)(reverse_ ? _draw_area.left() : (_draw_area.right() + 1)) / _graph_resource->size().x;
			_vertexes[2].v = (float)_draw_area.top() / _graph_resource->size().y;
			//����
			_vertexes[3].u = (float)(reverse_ ? (_draw_area.right() + 1) : _draw_area.left()) / _graph_resource->size().x;
			_vertexes[3].v = (float)_draw_area.top() / _graph_resource->size().y;
		}

	}
}