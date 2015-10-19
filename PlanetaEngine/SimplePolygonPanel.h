#pragma once

#pragma once

#include <memory>
#include <vector>
#include <array>
#include "UIComponent.h"
#include "Color.h"

namespace planeta_engine {
	namespace ui_components {
		class SimplePolygonPanel : public game::UIComponent {
		public:
			void SetNumberOfVertexes(size_t n);
			/*�w��C���f�b�N�X�̍��W��ݒ�B���W�̓R���|�[�l���g�̍����(0.0,0.0)�A�E����(1.0,1.0)�Ƃ������W�Ŏ����B*/
			bool SetVertexPosition(size_t idx, const Vector2D<double>& pos);
			/*���_��ݒ�B���W�̓R���|�[�l���g�̍����(0.0,0.0)�A�E����(1.0,1.0)�Ƃ������W�Ŏ����B*/
			void SetVertexes(const std::vector<Vector2D<double>>& vs);
			/*�A�N�Z�T*/
			/*�h��Ԃ��F���擾*/
			const core::Color& fill_color()const { return fill_color_; }
			/*�h��Ԃ��F��ݒ�*/
			void fill_color(const core::Color& c) { fill_color_ = c; }
			/*����`�悷�邩�̃t���O���擾*/
			bool is_draw_edge()const { return is_draw_outline_; }
			/*����`�悷�邩�̃t���O��ݒ�*/
			void is_draw_edge(bool f) { is_draw_outline_ = f; }
			/*�g���̐F���擾*/
			const core::Color& outline_color()const { return outline_color_; }
			/*�����̐F��ݒ�*/
			void outline_color(const core::Color& c) { outline_color_ = c; }
			/*�����̑������擾*/
			const int outline_width()const { return outline_width_; }
			/*�g���̑�����ݒ�*/
			void outline_width(int w) { outline_width_ = w; }
		private:
			virtual void UpdateProc() override;
			virtual void DrawProc(const utility::RectAngle<int>& draw_area) override;
			bool points_update_flag_ = true;
			/*Polygon���*/
			core::Color fill_color_;
			std::vector<Vector2D<double>> relative_points_;
			std::vector<Vector2D<int>> absolute_points_;
			std::vector<std::array<int, 3>> indexes_;
			void UpdateAbsolutePoints_(const utility::RectAngle<int>& draw_area);
			void UpdatePorygonIndex_();
			/*�����*/
			bool is_draw_outline_ = false;
			core::Color outline_color_;
			int outline_width_;

			void OnRectAngleUpdated()override;
		};
	}
}
