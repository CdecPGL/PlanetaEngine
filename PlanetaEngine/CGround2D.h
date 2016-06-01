#pragma once

#include <list>
#include "GameObjectStandardComponent.h"
#include "MathMatrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider2D.h"

namespace planeta_engine {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CTransform2D;
	class CGround2D :
		public GameObjectStandardComponent, public core::IColliderWithCollider2D {
	public:
		CGround2D();
		virtual ~CGround2D() = default;
		/*�w��n�`���W�̒n�ʂ���̍������擾*/
		virtual double GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos)const = 0;
		/*�O���[�o�����W��n�`���W�ɕϊ�*/
		virtual Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd& global_pos)const = 0;
		/*�n�`���W���O���[�o�����W�ɕϊ�*/
		virtual Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos)const = 0;
		/*�w��n�`���W�ŃO���[�o����]�x��n�`��]�x�ɕϊ�*/
		double ConvertRotationGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, double global_rota_rad)const;
		/*�w��n�`���W�Œn�`��]�x���O���[�o����]�x�ɕϊ�*/
		double ConvertRotationGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, double ground_rota_rad)const;
		/*�w��n�`���W�ŃO���[�o�����x��n�`���x�ɕϊ�*/
		Vector2Dd ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& global_velocity);
		/*�w��n�`���W�Œn�`���x���O���[�o�����x�ɕϊ�*/
		Vector2Dd ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_velocity);
		/*�w��n�`���W�Œn�`���W�n�x�N�g�����O���[�o�����W�n�ł̒����ƈ�v����悤�ϊ�*/
		virtual Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector)const = 0;

		CTransform2D& transform2d();
	protected:
		util::NonOwingPointer<CTransform2D> transform2d_;

		virtual bool OnInitialized() override;
		virtual bool OnActivated() override;
		virtual bool OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	private:
		/*�w��n�`���W�Œn�`���W�n��]�x�̃O���[�o�����W�n��]�x�Ƃ̍����擾����*/
		virtual double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos)const = 0;
	};
}
