#include "CDrawPlanet2D.h"
#include <cmath>
#include "ScreenDrawer2D.h"
#include "ResourceManager.h"
#include "RGraph.h"
#include "IGameObjectForComponent.h"
#include "CPlanet.h"
#include "MathConstant.h"
#include "SystemLog.h"
#include "GraphDrawData2D.h"

namespace {
	constexpr unsigned int kDefaultHorizontalSeparation(90);
	constexpr unsigned int kDefaultVerticalSeparation(5);
}

namespace planeta {

	CDrawPlanet2D::CDrawPlanet2D() :_horizontal_separation(kDefaultHorizontalSeparation), _vertical_separation(kDefaultVerticalSeparation), graph_draw_data_(std::make_shared<core::GraphDrawData2D>()) {
		texture_mapping_mode(TextureMappingMode::Round);
	}

	CDrawPlanet2D::~CDrawPlanet2D() = default;

	void CDrawPlanet2D::DrawProc(ScreenDrawer2D& drawer) {
		if (set_polygon_flag_) {
			SetPolygon_();
			set_polygon_flag_ = false;
		}
		UpdatePolygon_();
		drawer.DrawGraph(graph_draw_data_);
	}

	bool CDrawPlanet2D::GetOtherComponentProc(const GOComponentGetter& com_getter) {
		if (CDraw2D::GetOtherComponentProc(com_getter) == false) { return false; }
		_planet_component.reset(com_getter.GetComponent<CPlanet>());
		if (_planet_component) {
			SetPolygon_();
			return true;
		} else {
			PE_LOG_ERROR("�������Ɏ��s���܂����BPlanetComponent���擾�ł��܂���ł����B");
			return false;
		}
	}

	bool CDrawPlanet2D::OnInitialized() {
		if (CDraw2D::OnInitialized() == false) { return false; }
		return true;
	}

	void CDrawPlanet2D::OnFinalized()noexcept {

	}

	void CDrawPlanet2D::SetPolygonRoundly_() {
		//���_�ƃC���f�b�N�X�̃T�C�Y����
		graph_draw_data_->SetVertexCount((_vertical_separation + 1)*(_horizontal_separation + 1));
		graph_draw_data_->SetPlygonCount((_vertical_separation - 1)*_horizontal_separation * 2 + _horizontal_separation);
		//�摜�ƒ��_�̐ݒ�
		for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) { //���������͍��W�n�����ɃZ�b�g���Ă���
			for (unsigned int j = 0; j < _vertical_separation + 1; ++j) { //���������͏ォ�珇�ɃZ�b�g���Ă���
				//�e�N�X�`�����W�̃Z�b�g
				Vector2Df uv;
				uv.x = (float)i / _horizontal_separation;
				uv.y = (float)j / _vertical_separation;
				graph_draw_data_->SetVertexUV((_vertical_separation + 1)*i + j, uv);
			}
		}
		//�C���f�b�N�X�̐ݒ�
		for (unsigned int i = 0; i < _horizontal_separation; ++i) {
			//���S�ȊO�̓|���S����2��������
			for (unsigned int j = 0; j < _vertical_separation - 1; ++j) {
				core::GraphDrawData2D::PolygonIndexType poly1, poly2;
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

	void CDrawPlanet2D::UpdatePolygonRoundly_() {
		//���̂Ƃ���g��x�͍l�����Ă��Ȃ�
		Vector2Dd center_pos = GetDrawCenterPosition();
		double rotation = GetDrawRotationRed();
		//���_���W�̎Z�o
		for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) {
			double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //���݂̘f�����_�̈ʒu�p�x
			double height = _planet_component->GetHeightByRad(angle_rad); //���݂̘f�����_�̈ʒu�p�x�̍���
			Vector2Dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //���S����n�\�̌��݊p�x�ւ̃x�N�g��
			interface_vec *= height;
			//���S�ȊO�̒��_���W�����߂�
			for (unsigned int j = 0; j < _vertical_separation; ++j) {
				double dis_ratio = 1.0f - (double)j / _vertical_separation;
				graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + j, static_cast<Vector2Df>(center_pos + interface_vec*dis_ratio));
			}
			//���S�̒��_���W�����߂�
			graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + _vertical_separation, static_cast<Vector2Df>(center_pos));
		}
		//���_�F�ݒ�
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->SetVertexColor(i, color());
		}
	}

	void CDrawPlanet2D::SetPolygonPlainly_() {
		//���_�ƃC���f�b�N�X�̃T�C�Y����
		graph_draw_data_->SetVertexCount(_horizontal_separation + 1);
		graph_draw_data_->SetPlygonCount(_horizontal_separation);
		//�摜�ƒ��_�̐ݒ�
		for (unsigned int i = 0; i < _horizontal_separation; ++i) { //���������͍��W�n�����ɃZ�b�g���Ă���
			double rad = math::PI * 2 / _horizontal_separation * i;
			//�e�N�X�`�����W�̃Z�b�g
			Vector2Df uv;
			uv.x = 0.5f + (float)std::cos(rad)*0.5f;
			uv.y = 0.5f + (float)std::sin(rad)*0.5f;
			graph_draw_data_->SetVertexUV(i, uv);
		}
		graph_draw_data_->SetVertexUV(_horizontal_separation, Vector2Df(0.5f, 0.5f)); //�Ō�̒��_�͒��S���w��
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

	void CDrawPlanet2D::UpdatePolygonPlainly_() {
		//���̂Ƃ���g��x�͍l�����Ă��Ȃ�
		Vector2Dd center_pos = GetDrawCenterPosition();
		double rotation = GetDrawRotationRed();
		//���_���W�̎Z�o
		for (unsigned int i = 0; i < _horizontal_separation; ++i) {
			double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //���݂̘f�����_�̈ʒu�p�x
			double height = _planet_component->GetHeightByRad(angle_rad); //���݂̘f�����_�̈ʒu�p�x�̍���
			Vector2Dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //���S����n�\�̌��݊p�x�ւ̃x�N�g��
			interface_vec *= height;
			graph_draw_data_->SetVertexPosition(i, static_cast<Vector2Df>(center_pos + interface_vec));
		}
		//���S���_�̐ݒ�
		graph_draw_data_->SetVertexPosition(_horizontal_separation, static_cast<Vector2Df>(center_pos));
		//���_�F�ݒ�
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->SetVertexColor(i, color());
		}
	}

	bool CDrawPlanet2D::SetGraphResource(const std::string& resource_id) {
		auto res = core::ResourceManager::instance().GetResource(resource_id);
		if (res == nullptr) {
			PE_LOG_ERROR("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����", resource_id, ")");
			return false;
		}
		std::shared_ptr<RGraph> gr = std::dynamic_pointer_cast<RGraph>(res);
		if (gr) {
			graph_draw_data_->SetGraphResource(gr);
			set_polygon_flag_ = true;
			return true;
		} else {
			PE_LOG_ERROR("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����", resource_id, "�A�^�C�v��", res->GetType().name(), ")");
			return false;
		}
	}

	CDrawPlanet2D& CDrawPlanet2D::horizontal_separation(unsigned int sep) {
		_horizontal_separation = sep;
		set_polygon_flag_ = true;
		return *this;
	}
	CDrawPlanet2D& CDrawPlanet2D::vertical_separation(unsigned int sep) {
		_vertical_separation = sep;
		set_polygon_flag_ = true;
		return *this;
	}

	void CDrawPlanet2D::texture_mapping_mode(TextureMappingMode tmm) {
		tex_map_mode_ = tmm;
		switch (tex_map_mode_) {
		case CDrawPlanet2D::TextureMappingMode::Round:
			polygon_setter_ = &CDrawPlanet2D::SetPolygonRoundly_;
			polygon_updater_ = &CDrawPlanet2D::UpdatePolygonRoundly_;
			break;
		case CDrawPlanet2D::TextureMappingMode::Plain:
			polygon_setter_ = &CDrawPlanet2D::SetPolygonPlainly_;
			polygon_updater_ = &CDrawPlanet2D::UpdatePolygonPlainly_;
			break;
		default:
			break;
		}
		set_polygon_flag_ = true;
	}

	void CDrawPlanet2D::SetPolygon_() {
		(this->*polygon_setter_)();
	}

	void CDrawPlanet2D::UpdatePolygon_() {
		(this->*polygon_updater_)();
	}
}