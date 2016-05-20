#pragma once
#include "GameObjectSystemComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class CTransform2D;
	class CDraw2D : public core::GameObjectSystemComponent {
	public:
		CDraw2D();
		virtual ~CDraw2D() = default;
		/*�`�揈��*/
		void Draw(ScreenDrawer2D& drawer) { DrawProc(drawer); }
		/*Accessor*/
		/*�`��D��x���擾*/
		int draw_priority()const { return draw_priority_; }
		/*�`��D��x��ݒ�*/
		CDraw2D& draw_priority(int priority);
		/*�\���ʒu���擾*/
		const Vector2Dd& position()const { return position_; }
		/*�\���ʒu��ݒ�*/
		CDraw2D& position(const Vector2Dd& pos) { position_ = pos; return *this; }
		/*�\����]�x���擾*/
		double rotation_rad()const { return rotation_rad_; }
		/*�\����]�x��ݒ�*/
		CDraw2D& rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
		/*�\���g��x���擾*/
		const Vector2Dd& scale()const { return scale_; }
		/*�\���g��x��ݒ�*/
		CDraw2D& scale(const Vector2Dd& s) { scale_ = s; return *this; }
		/*�\���F���擾*/
		planeta_engine::Color color()const { return color_; }
		/*�\���F��ݒ�*/
		CDraw2D& color(const planeta_engine::Color& c) { color_ = c; return *this; }
	protected:
		/*�`��̒��S�ʒu�擾(�Q�[���I�u�W�F�N�g�̌`����ƁA�\���ʒu����Z�o)*/
		Vector2Dd GetDrawCenterPosition()const;
		/*�`���]�x�擾(�Q�[���I�u�W�F�N�g�̉�]�x�ƁA�\����]�x����Z�o)*/
		double GetDrawRotationRed()const;
		/*�`��g��x�擾(�Q�[���I�u�W�F�N�g�̊g��x�ƁA�\����g��x����Z�o)*/
		Vector2Dd GetDrawScale()const;

		utility::NonOwingPointer<CTransform2D> transform2d_;

		bool OnInitialized()override;
	private:
		int draw_priority_ = 0; //�`��D��x
		/*�\���ʒu*/
		Vector2Dd position_;
		/*�\����]�x*/
		double rotation_rad_ = 0.0;
		/*�\���g��x*/
		Vector2Dd scale_ = Vector2Dd(1.0, 1.0);
		planeta_engine::Color color_;
		void RegisterToProcess_();
		void RemoveFromProcess_();
		void UpdatePriority_();
		bool OnActivated()override final;
		bool OnInactivated()override final;
		/*�`�揈��*/
		virtual void DrawProc(ScreenDrawer2D& drawer) = 0;
	};
}
