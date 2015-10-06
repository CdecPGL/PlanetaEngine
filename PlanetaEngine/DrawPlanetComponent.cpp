#include "DrawPlanetComponent.h"
#include <cmath>
#include "DrawManager.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "IGameObjectAccessor.h"
#include "PlanetComponent.h"
#include "MathConstant.h"
#include "SystemLog.h"

namespace {
	constexpr unsigned int kDefaultHorizontalSeparation(90);
	constexpr unsigned int kDefaultVerticalSeparation(5);
}

namespace planeta_engine {
	namespace components {

		DrawPlanetComponent::DrawPlanetComponent() :_horizontal_separation(kDefaultHorizontalSeparation), _vertical_separation(kDefaultVerticalSeparation)
		{

		}

		void DrawPlanetComponent::Draw()
		{
			_UpdatePolygon();
			core::DrawManager::instance().DrawGraph(_vertexes, _indexes, _graph_resource);
		}

		bool DrawPlanetComponent::Initialize_()
		{
			if (DrawComponent::Initialize_() == false) { return false; }
			_planet_component = game_object()->GetComponent<components::PlanetComponent>();
			if (_planet_component) {
				_SetPolygon();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("�������Ɏ��s���܂����BPlanetComponent���擾�ł��܂���ł����B", "DrawPlanetComponent::Initialize_");
				return false;
			}
		}

		void DrawPlanetComponent::Finalize_()
		{
			DrawComponent::Finalize_();
		}

		void DrawPlanetComponent::_SetPolygon()
		{
			//���_�ƃC���f�b�N�X�̃T�C�Y����
			_vertexes.resize((_vertical_separation + 1)*(_horizontal_separation + 1));
			_indexes.resize((_vertical_separation - 1)*_horizontal_separation * 2 + _horizontal_separation);
			//�摜�ƒ��_�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) { //���������͍��W�n�����ɃZ�b�g���Ă���
				for (unsigned int j = 0; j < _vertical_separation + 1; ++j) { //���������͏ォ�珇�ɃZ�b�g���Ă���
					//�e�N�X�`�����W�̃Z�b�g
					_vertexes[(_vertical_separation + 1)*i + j].u = (float)i / _horizontal_separation;
					_vertexes[(_vertical_separation + 1)*i + j].v = (float)j / _vertical_separation;
				}
			}
			//�C���f�b�N�X�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation; ++i) {
				//���S�ȊO�̓|���S����2��������
				for (unsigned int j = 0; j < _vertical_separation - 1; ++j) {
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][0] = i*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][1] = i*(_vertical_separation + 1) + j + 1;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][2] = (i + 1)*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][0] = i*(_vertical_separation + 1) + j + 1;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][1] = (i + 1)*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][2] = (i + 1)*(_vertical_separation + 1) + j + 1;
				}
				//���S�̓|���S����1����������
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][0] = i*(_vertical_separation + 1) + (_vertical_separation - 1);
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][1] = i*(_vertical_separation + 1) + (_vertical_separation - 1) + 1;
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][2] = (i + 1)*(_vertical_separation + 1) + (_vertical_separation - 1);
			}
		}

		void DrawPlanetComponent::_UpdatePolygon()
		{
			if (game_object() == nullptr || _graph_resource == nullptr || _planet_component == nullptr) { return; }
			//���̂Ƃ���g��x�͍l�����Ă��Ȃ�
			Vector2D<double> center_pos = GetDrawCenterPosition();
			double rotation = GetDrawRotationRed();
			//���_���W�̎Z�o
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) {
				double angle_rad = 2.0 * math::constant::PI / _horizontal_separation * i; //���݂̘f�����_�̈ʒu�p�x
				double height = _planet_component->GetHeightByRad(angle_rad); //���݂̘f�����_�̈ʒu�p�x�̍���
				Vector2D<double> interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //���S����n�\�̌��݊p�x�ւ̃x�N�g��
				interface_vec *= height;
				//���S�ȊO�̒��_���W�����߂�
				for (unsigned int j = 0; j < _vertical_separation; ++j) {
					double dis_ratio = 1.0f - (double)j / _vertical_separation;
					_vertexes[i*(_vertical_separation + 1) + j].position = center_pos + interface_vec*dis_ratio;
				}
				//���S�̒��_���W�����߂�
				_vertexes[i*(_vertical_separation + 1) + _vertical_separation].position = center_pos;
			}
			//���_�F�ݒ�
			for (auto& vtx : _vertexes) {
				vtx.color = color();
			}
		}

		bool DrawPlanetComponent::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����") + resource_id + ")", "DrawPlanetComponent::SetGraphResource");
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				_graph_resource = gr;
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����") + resource_id + "�A�^�C�v��" + res->GetType().name() + ")", "DrawPlanetComponent::SetGraphResource");
				return false;
			}
		}

		void DrawPlanetComponent::horizontal_separation(unsigned int sep)
		{
			_horizontal_separation = sep;
			_SetPolygon();
		}
		void DrawPlanetComponent::vertical_separation(unsigned int sep)
		{
			_vertical_separation = sep;
			_SetPolygon();
		}
	}
}