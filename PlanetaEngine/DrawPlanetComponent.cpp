#include "DrawPlanetComponent.h"
#include <cmath>
#include "ScreenDrawer2D.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "IGameObjectAccessor.h"
#include "PlanetComponent.h"
#include "MathConstant.h"
#include "SystemLog.h"
#include "GraphDrawData2D.h"

namespace {
	constexpr unsigned int kDefaultHorizontalSeparation(90);
	constexpr unsigned int kDefaultVerticalSeparation(5);
}

namespace planeta_engine {
	namespace components {

		DrawPlanetComponent::DrawPlanetComponent() :_horizontal_separation(kDefaultHorizontalSeparation), _vertical_separation(kDefaultVerticalSeparation),graph_draw_data_(std::make_shared<core::GraphDrawData2D>())
		{
			texture_mapping_mode(TextureMappingMode::Round);
		}

		DrawPlanetComponent::~DrawPlanetComponent() = default;

		void DrawPlanetComponent::DrawProc(ScreenDrawer2D& drawer)
		{
			if (set_polygon_flag_) {
				SetPolygon_(); 
				set_polygon_flag_ = false;
			}
			UpdatePolygon_();
			drawer.DrawGraph(graph_draw_data_);
		}

		bool DrawPlanetComponent::OnInitialized()
		{
			_planet_component = game_object().GetComponent<components::PlanetComponent>();
			if (_planet_component) {
				SetPolygonRoundly_();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("�������Ɏ��s���܂����BPlanetComponent���擾�ł��܂���ł����B", "DrawPlanetComponent::Initialize_");
				return false;
			}
		}

		void DrawPlanetComponent::OnFinalized()noexcept
		{
			
		}

		void DrawPlanetComponent::SetPolygonRoundly_()
		{
			//���_�ƃC���f�b�N�X�̃T�C�Y����
			graph_draw_data_->SetVertexCount((_vertical_separation + 1)*(_horizontal_separation + 1));
			graph_draw_data_->SetPlygonCount((_vertical_separation - 1)*_horizontal_separation * 2 + _horizontal_separation);
			//�摜�ƒ��_�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) { //���������͍��W�n�����ɃZ�b�g���Ă���
				for (unsigned int j = 0; j < _vertical_separation + 1; ++j) { //���������͏ォ�珇�ɃZ�b�g���Ă���
					//�e�N�X�`�����W�̃Z�b�g
					Vector2D<float> uv;
					uv.x = (float)i / _horizontal_separation;
					uv.y = (float)j / _vertical_separation;
					graph_draw_data_->SetVertexUV((_vertical_separation + 1)*i + j, uv);
				}
			}
			//�C���f�b�N�X�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation; ++i) {
				//���S�ȊO�̓|���S����2��������
				for (unsigned int j = 0; j < _vertical_separation - 1; ++j) {
					core::GraphDrawData2D::PolygonIndexType poly1,poly2;
					poly1[0] = i*(_vertical_separation + 1) + j;
					poly1[1] = i*(_vertical_separation + 1) + j + 1;
					poly1[2] = (i + 1)*(_vertical_separation + 1) + j;
					poly2[0] = i*(_vertical_separation + 1) + j + 1;
					poly2[1] = (i + 1)*(_vertical_separation + 1) + j;
					poly2[2] = (i + 1)*(_vertical_separation + 1) + j + 1;
					graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + j * 2, poly1);
					graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + j * 2 + 1, poly2);
				}
				//���S�̓|���S����1����������
				core::GraphDrawData2D::PolygonIndexType poly;
				poly[0] = i*(_vertical_separation + 1) + (_vertical_separation - 1);
				poly[1] = i*(_vertical_separation + 1) + (_vertical_separation - 1) + 1;
				poly[2] = (i + 1)*(_vertical_separation + 1) + (_vertical_separation - 1);
				graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2, poly);
			}
		}

		void DrawPlanetComponent::UpdatePolygonRoundly_()
		{
			//���̂Ƃ���g��x�͍l�����Ă��Ȃ�
			Vector2D<double> center_pos = GetDrawCenterPosition();
			double rotation = GetDrawRotationRed();
			//���_���W�̎Z�o
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) {
				double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //���݂̘f�����_�̈ʒu�p�x
				double height = _planet_component->GetHeightByRad(angle_rad); //���݂̘f�����_�̈ʒu�p�x�̍���
				Vector2D<double> interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //���S����n�\�̌��݊p�x�ւ̃x�N�g��
				interface_vec *= height;
				//���S�ȊO�̒��_���W�����߂�
				for (unsigned int j = 0; j < _vertical_separation; ++j) {
					double dis_ratio = 1.0f - (double)j / _vertical_separation;
					graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + j, center_pos + interface_vec*dis_ratio);
				}
				//���S�̒��_���W�����߂�
				graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + _vertical_separation, center_pos);
			}
			//���_�F�ݒ�
			for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
				graph_draw_data_->SetVertexColor(i, color());
			}
		}

		void DrawPlanetComponent::SetPolygonPlainly_() {
			//���_�ƃC���f�b�N�X�̃T�C�Y����
			graph_draw_data_->SetVertexCount(_horizontal_separation + 1);
			graph_draw_data_->SetPlygonCount(_horizontal_separation);
			//�摜�ƒ��_�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation; ++i) { //���������͍��W�n�����ɃZ�b�g���Ă���
				double rad = math::PI * 2 / _horizontal_separation * i;
				//�e�N�X�`�����W�̃Z�b�g
				Vector2D<float> uv;
				uv.x = 0.5f + (float)std::cos(rad)*0.5f;
				uv.y = 0.5f + (float)std::sin(rad)*0.5f;
				graph_draw_data_->SetVertexUV(i, uv);
			}
			graph_draw_data_->SetVertexUV(_horizontal_separation, Vector2D<float>(0.5f, 0.5f)); //�Ō�̒��_�͒��S���w��
			//�C���f�b�N�X�̐ݒ�
			for (unsigned int i = 0; i < _horizontal_separation; ++i) {
				//�|���S����1��������
				core::GraphDrawData2D::PolygonIndexType poly1;
				poly1[0] = i;
				poly1[1] = (i + 1) % _horizontal_separation;
				poly1[2] = _horizontal_separation;
				graph_draw_data_->SetPolyginIndex(i, poly1);
			}
		}

		void DrawPlanetComponent::UpdatePolygonPlainly_() {
			//���̂Ƃ���g��x�͍l�����Ă��Ȃ�
			Vector2D<double> center_pos = GetDrawCenterPosition();
			double rotation = GetDrawRotationRed();
			//���_���W�̎Z�o
			for (unsigned int i = 0; i < _horizontal_separation; ++i) {
				double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //���݂̘f�����_�̈ʒu�p�x
				double height = _planet_component->GetHeightByRad(angle_rad); //���݂̘f�����_�̈ʒu�p�x�̍���
				Vector2D<double> interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //���S����n�\�̌��݊p�x�ւ̃x�N�g��
				interface_vec *= height;
				graph_draw_data_->SetVertexPosition(i, center_pos + interface_vec);
			}
			//���S���_�̐ݒ�
			graph_draw_data_->SetVertexPosition(_horizontal_separation, center_pos);
			//���_�F�ݒ�
			for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
				graph_draw_data_->SetVertexColor(i, color());
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
				graph_draw_data_->SetGraphResource (gr);
				set_polygon_flag_ = true;
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����") + resource_id + "�A�^�C�v��" + res->GetType().name() + ")", "DrawPlanetComponent::SetGraphResource");
				return false;
			}
		}

		DrawPlanetComponent& DrawPlanetComponent::horizontal_separation(unsigned int sep)
		{
			_horizontal_separation = sep;
			set_polygon_flag_ = true;
			return *this;
		}
		DrawPlanetComponent& DrawPlanetComponent::vertical_separation(unsigned int sep)
		{
			_vertical_separation = sep;
			set_polygon_flag_ = true;
			return *this;
		}

		void DrawPlanetComponent::texture_mapping_mode(TextureMappingMode tmm) {
			tex_map_mode_ = tmm;
			switch (tex_map_mode_) {
			case planeta_engine::components::DrawPlanetComponent::TextureMappingMode::Round:
				polygon_setter_ = &DrawPlanetComponent::SetPolygonRoundly_;
				polygon_updater_ = &DrawPlanetComponent::UpdatePolygonRoundly_;
				break;
			case planeta_engine::components::DrawPlanetComponent::TextureMappingMode::Plain:
				polygon_setter_ = &DrawPlanetComponent::SetPolygonPlainly_;
				polygon_updater_ = &DrawPlanetComponent::UpdatePolygonPlainly_;
				break;
			default:
				break;
			}
			set_polygon_flag_ = true;
		}

		void DrawPlanetComponent::SetPolygon_() {
			(this->*polygon_setter_)();
		}

		void DrawPlanetComponent::UpdatePolygon_() {
			(this->*polygon_updater_)();
		}
	}
}