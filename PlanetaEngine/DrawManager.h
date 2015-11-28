#pragma once
#include <vector>
#include <array>
#include <memory>
#include "PointerSingletonTemplate.h"
#include "Vector2D.h"
#include "Color.h"
#include "RectAngle.h"
#include "Vertex2D.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
		class FontDefinitionResource;
	}
	namespace core {
		class ScreenEffecter;
		class GraphDrawData;
		class DrawManager final : public utility::PointerSingletonTemplate<DrawManager>{
			friend utility::PointerSingletonTemplate<DrawManager>;
		public:
			bool Initialize() override;
			bool Finalize() override;

			//�V�X�e���֐�
			bool Update();
			//�`��֐�
			/*�摜�̕`��(�摜�`��f�[�^)*/
			void DrawGraph(const GraphDrawData& graph_draw_data);
			/*���C���[�̕`��(�_���W�A��(����ł͖���)�A�F)*/
			void DrawWire(const std::vector<Vector2D<double>>& positions, double width, const Color& color);
			/*���p�`�̕`��(�_���W�A�O�p�`�|���S���C���f�b�N�X�A�F)*/
			void DrawPolygon(const std::vector<Vector2D<double>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color);
			//�J�����֐�
			/*�J�����ʒu��ݒ�*/
			void SetCameraPosition(const Vector2D<double>& position);
			/*�J������]�x��ݒ�*/
			void SetCameraRotationRad(double rotation_rad);
			/*�J�����g��x��ݒ�*/
			void SetCameraScale(double scale);

			//UI�`��֐�
			/*UI�摜��`��(�`��͈́A�摜��̕`��͈́A���]�t���O�A�摜���\�[�X)*/
			void DrawUIGraph(const utility::RectAngle<int>& draw_area, const utility::RectAngle<int>& draw_area_on_graph,bool reverse, const std::shared_ptr<resources::GraphResource>& graph_resource);
			/*UI���C���[��`��(�_���W�A��(����ł͖���)�A�F)*/
			void DrawUIWire(const std::vector<Vector2D<int>>& positions, int width, const Color& color);
			/*UI���p�`��`��(�_���W�A�O�p�`�|���S���C���f�b�N�X�A�F)*/
			void DrawUIPolygon(const std::vector<Vector2D<int>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color);
			/*�������`��(�`��ʒu�A�g��x�A�`�敶����A�F�A���F�A�t�H���g��`���\�[�X)*/
			void DrawUIString(const Vector2D<int>& position, const Vector2D<double> scale, const std::string& str, const core::Color& color, const core::Color& outline_color, const std::shared_ptr<resources::FontDefinitionResource>& font_definition_resource);

			//���݂̃X�N���[���ɃG�t�F�N�g���|����
			bool ApplyEffectToScreen(ScreenEffecter& screen_effecter);
		private:
			DrawManager() = default;
			~DrawManager() = default;
			static Vector2D<double> _ConvertPosition(const Vector2D<double>& position);

			Vector2D<double> camera_posision_;
			double camera_rotation_rad_ = 0.0;
			double camera_scale_ = 1.0;
		};
	}
}
