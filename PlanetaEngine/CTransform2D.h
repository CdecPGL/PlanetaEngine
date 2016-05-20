#pragma once

#include <unordered_map>
#include "GameObjectSystemComponent.h"
#include "Vector2D.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	class Transform2DCore;
	class CGround2D;
	/*�`����R���|�[�l���g*/
	class CTransform2D : public core::GameObjectSystemComponent {
	public:
		CTransform2D();
		~CTransform2D();

		/*���݈ʒu����ړ�*/
		void Move(const Vector2Dd& mov_pos);
		/*�ʒu�̐ݒ�*/
		void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset);
		/*��������*/
		void Accelerate(const Vector2Dd& acceleration);
		/*�O���[�o�������ɉ�������*/
		void GlobalAccelerate(const Vector2Dd& global_acceleration);
		/*�A�N�Z�T*/
		const Vector2Dd& position()const;
		CTransform2D& position(const Vector2Dd& pos);
		const Vector2Dd& scale()const;
		CTransform2D&  scale(const Vector2Dd& s);
		const double rotation_rad()const;
		CTransform2D&  rotation_rad(double rota_rad);

		const Vector2Dd& global_position()const;
		CTransform2D&  global_position(const Vector2Dd& pos);
		const double global_rotation_rad()const;
		CTransform2D&  global_rotation_rad(double rota_rad);

		const Vector2Dd& velocity()const;
		CTransform2D&  velocity(const Vector2Dd& vel);
		const double rotation_velocity_rad()const;
		CTransform2D&  rotation_velocity_rad(double rota_vel_rad);

		const Vector2Dd& global_velocity()const;
		CTransform2D&  global_velocity(const Vector2Dd& vel);

		/*�n�`�֌W*/
		const CGround2D& ground()const;
		CGround2D& ground();
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		utility::WeakPointer<CGround2D> GetGround()const;
		void SetGround(const utility::WeakPointer<CGround2D>& g, bool keep_global_position);

		//�V�X�e��
		void ApplyVelocity();
	private:
		bool is_no_update()const override { return true; }

		std::unique_ptr<Transform2DCore> core_;
	};
}
