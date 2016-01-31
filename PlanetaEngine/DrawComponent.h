#pragma once
#include "GameObjectSpecialComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	namespace system_processes {
		class GameObjectDrawProcessCore;
	}
	namespace core {
//		class GameObjectDrawComponentProcessRegistrator;
	}
	namespace components {
		class DrawComponent : public core::GameObjectSpecialComponent {
		public:
			DrawComponent();
			virtual ~DrawComponent() = default;
			/*�`�揈��*/
			void Draw(ScreenDrawer2D& drawer) { DrawProc(drawer); }
			/*Accessor*/
			/*�`��D��x���擾*/
			int draw_priority()const { return draw_priority_; }
			/*�`��D��x��ݒ�*/
			DrawComponent& draw_priority(int priority);
			/*�\���ʒu���擾*/
			const Vector2D<double>& position()const { return position_; }
			/*�\���ʒu��ݒ�*/
			DrawComponent& position(const Vector2D<double>& pos) { position_ = pos; return *this; }
			/*�\����]�x���擾*/
			double rotation_rad()const { return rotation_rad_; }
			/*�\����]�x��ݒ�*/
			DrawComponent& rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
			/*�\���g��x���擾*/
			const Vector2D<double>& scale()const { return scale_; }
			/*�\���g��x��ݒ�*/
			DrawComponent& scale(const Vector2D<double>& s) { scale_ = s; return *this; }
			/*�\���F���擾*/
			planeta_engine::Color color()const { return color_; }
			/*�\���F��ݒ�*/
			DrawComponent& color(const planeta_engine::Color& c) { color_ = c; return *this; }
		protected:
			/*�`��̒��S�ʒu�擾(�Q�[���I�u�W�F�N�g�̌`����ƁA�\���ʒu����Z�o)*/
			Vector2D<double> GetDrawCenterPosition()const;
			/*�`���]�x�擾(�Q�[���I�u�W�F�N�g�̉�]�x�ƁA�\����]�x����Z�o)*/
			double GetDrawRotationRed()const;
			/*�`��g��x�擾(�Q�[���I�u�W�F�N�g�̊g��x�ƁA�\����g��x����Z�o)*/
			Vector2D<double> GetDrawScale()const;
		private:
			int draw_priority_ = 0; //�`��D��x
			/*�\���ʒu*/
			Vector2D<double> position_;
			/*�\����]�x*/
			double rotation_rad_ = 0.0;
			/*�\���g��x*/
			Vector2D<double> scale_ = Vector2D<double>(1.0, 1.0);
			planeta_engine::Color color_;
//			std::shared_ptr<core::GameObjectDrawComponentProcessRegistrator> draw_component_registrator_;
			utility::WeakPointer<system_processes::GameObjectDrawProcessCore> draw_component_registrator_;
			void RegisterToProcess_();
			void RemoveFromProcess_();
			void UpdatePriority_();
			/*����Z�b�g�A�b�v*/
			bool SpecialSetUp(const core::SceneDataForGameObject& setup_data)override final;
			/*�X�V�����͍s��Ȃ�*/
			bool is_no_update()const override final { return true; }
			bool OnActivated()override final;
			bool OnInactivated()override final;
			/*�`�揈��*/
			virtual void DrawProc(ScreenDrawer2D& drawer) = 0;
		};
	}
}
