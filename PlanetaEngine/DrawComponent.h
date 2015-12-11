#pragma once
#include "GameObjectComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace system_processes {
		class GameObjectDrawProcess;
	}
	namespace components {
		class DrawComponent : public game::GameObjectComponent {
		public:
			DrawComponent();
			virtual ~DrawComponent() = default;
			/*�`�揈��*/
			virtual void Draw() = 0;
			/*Accessor*/
			/*�`��D��x���擾*/
			int draw_priority()const { return _draw_priority; }
			/*�`��D��x��ݒ�*/
			void draw_priority(int priority);
			/*�\���ʒu���擾*/
			const Vector2D<double>& position()const { return _position; }
			/*�\���ʒu��ݒ�*/
			void position(const Vector2D<double>& pos) { _position = pos; }
			/*�\����]�x���擾*/
			double rotation_rad()const { return _rotation_rad; }
			/*�\����]�x��ݒ�*/
			void rotation_rad(double rota_rad) { _rotation_rad = rota_rad; }
			/*�\���g��x���擾*/
			const Vector2D<double>& scale()const { return _scale; }
			/*�\���g��x��ݒ�*/
			void scale(const Vector2D<double>& s) { _scale = s; }
			/*�\���F���擾*/
			core::Color color()const { return _color; }
			/*�\���F��ݒ�*/
			void color(const core::Color& c) { _color = c; }
		protected:
			/*�`��̒��S�ʒu�擾(�Q�[���I�u�W�F�N�g�̌`����ƁA�\���ʒu����Z�o)*/
			Vector2D<double> GetDrawCenterPosition()const;
			/*�`���]�x�擾(�Q�[���I�u�W�F�N�g�̉�]�x�ƁA�\����]�x����Z�o)*/
			double GetDrawRotationRed()const;
			/*�`��g��x�擾(�Q�[���I�u�W�F�N�g�̊g��x�ƁA�\����g��x����Z�o)*/
			Vector2D<double> GetDrawScale()const;
		protected:
			/*�V�X�e���֐�(���̃N���X���p������N���X�ňȉ��̊֐����I�[�o�[���C�h����ꍇ�ADrawComponent�̓��N���X���Ăяo���Ȃ���΂Ȃ�Ȃ��B)*/
			bool Initialize_()override;
			void Finalize_()override {};
		private:
			int _draw_priority = 0; //�`��D��x
			/*�\���ʒu*/
			Vector2D<double> _position;
			/*�\����]�x*/
			double _rotation_rad = 0.0;
			/*�\���g��x*/
			Vector2D<double> _scale = Vector2D<double>(1.0, 1.0);
			core::Color _color;
			utility::WeakPointer<system_processes::GameObjectDrawProcess> _game_object_draw_process;
			void _ResistToDrawProcess();
			void _RemoveFromDrawProcess();
			void _UpdatePriority();

			/*�V�X�e���֐�*/
			bool Activated_()override final;
			/*�X�V�����͍s��Ȃ�*/
			bool NoUpdate_()const override final { return true; }
			bool InActivated_()override final;
		};
	}
}
