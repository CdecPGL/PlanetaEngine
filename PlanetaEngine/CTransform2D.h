#pragma once

#include <unordered_map>
#include "GameObjectSystemComponent.h"
#include "Vector2D.h"
#include "Delegate.h"

namespace planeta_engine {
	class CGround2D;
	class IGameObject;
	/*�`����R���|�[�l���g*/
	class CTransform2D : public core::GameObjectSystemComponent {
	public:
		CTransform2D();
		~CTransform2D();

		/*�O���[�o����������*/
		void Accelerate(const Vector2Dd& acceleration);
		/*�n�`��������*/
		void GroundAccelerate(const Vector2Dd& ground_acceleration);
		/*���ݒn�`�ʒu����ړ�*/
		void GroundMove(const Vector2Dd& mov_pos);
		/*�n�`�ʒu�̐ݒ�*/
		void GroundOffset(const Vector2Dd& base_ground_pos, const Vector2Dd& offset);
		/*�A�N�Z�T*/
		//�O���[�o���ʒu���擾
		const Vector2Dd& position()const;
		//�O���[�o���ʒu��ݒ�
		CTransform2D& position(const Vector2Dd& pos);
		//�O���[�o���X�P�[�����擾
		const Vector2Dd& scale()const;
		//�O���[�o���X�P�[����ݒ�
		CTransform2D&  scale(const Vector2Dd& s);
		//�O���[�o����]�x���擾
		const double rotation_rad()const;
		//�O���[�o����]�x��ݒ�
		CTransform2D&  rotation_rad(double rota_rad);

		//�n�`�ʒu���擾
		const Vector2Dd& ground_position()const;
		//�n�`�ʒu��ݒ�
		CTransform2D& ground_position(const Vector2Dd& pos);
		//�n�`��]�x���擾
		const double ground_rotation_rad()const;
		//�n�`��]�x��ݒ�
		CTransform2D&  ground_rotation_rad(double rota_rad);

		//�O���[�o�����x���擾
		const Vector2Dd& velocity()const;
		//�O���[�o�����x��ݒ�
		CTransform2D&  velocity(const Vector2Dd& vel);
		//�O���[�o���p���x���擾
		const double rotation_velocity_rad()const;
		//�O���[�o���p���x��ݒ�
		CTransform2D&  rotation_velocity_rad(double rota_vel_rad);

		//�n�`���x���擾
		const Vector2Dd& ground_velocity()const;
		//�n�`���x��ݒ�
		CTransform2D&  ground_velocity(const Vector2Dd& vel);

		//���x��Ԃ�ݒ�
		enum class Space { Ground, Global };
		void velocity_space(Space space);

		/*�n�`�֌W*/
		const CGround2D& ground()const;
		CGround2D& ground();
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		utility::WeakPointer<CGround2D> GetGround()const;
		bool SetGround(const utility::WeakPointer<IGameObject>& g, bool keep_global_position);
		/*�g�����X�t�H�[���X�V�C�x���g�n���h���ǉ�*/
		utility::DelegateConnection AddUpdatedEventHandler(utility::DelegateHandlerAdder<void>&& handler_adder);

		//�V�X�e��
		void ApplyVelocity_(); //���x�K�p
	private:
		//����
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOC�I�[�o�[���C�h
		bool OnInitialized()override final;
		void OnFinalized()noexcept override final;
		bool OnActivated()override final;
		bool OnInactivated()override final;
	};
}
