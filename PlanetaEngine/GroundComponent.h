#pragma once
#include "boost/optional/optional.hpp"
#include "GameObjectNormalComponent.h"
#include "Matrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
		class GroundComponent :
			public GameObjectNormalComponent , public core::IColliderWithCollider
		{
		public:
			GroundComponent();
			virtual ~GroundComponent() = default;
			/*�w��n�`���W�̒n�ʂ���̍������擾*/
			virtual double GetAltitudeWithGroundPosition(const Vector2D<double>& ground_pos)const = 0;
			/*�O���[�o�����W��n�`���W�ɕϊ�*/
			virtual Vector2D<double> ConvertPositionGlobalToGround(const Vector2D<double>& global_pos)const = 0;
			/*�n�`���W���O���[�o�����W�ɕϊ�*/
			virtual Vector2D<double> ConvertPositionGroundToGlobal(const Vector2D<double>& ground_pos)const = 0;
			/*�w��n�`���W�ŃO���[�o����]�x��n�`��]�x�ɕϊ�*/
			double ConvertRotationGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, double global_rota_rad)const;
			/*�w��n�`���W�Œn�`��]�x���O���[�o����]�x�ɕϊ�*/
			double ConvertRotationGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, double ground_rota_rad)const;
			/*�w��n�`���W�ŃO���[�o�����x��n�`���x�ɕϊ�*/
			Vector2D<double> ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& global_velocity);
			/*�w��n�`���W�Œn�`���x���O���[�o�����x�ɕϊ�*/
			Vector2D<double> ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_velocity);
			/*�w��n�`���W�Œn�`���W�n�x�N�g�����O���[�o�����W�n�ł̒����ƈ�v����悤�ϊ�*/
			virtual Vector2D<double> NormalizeGroundVectorWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_vector)const = 0;
			/*X���W�͈͎̔擾*/
			//const std::pair<double,double>& GetXRange()const;
			/*Y���W�̎����擾*/
			//const std::pair<double, double>& GetYRange()const;
			/*X���W��*/
		protected:
			//void SetXRange(const std::pair<double, double>& range);
			//void SetYRange(const std::pair<double, double>& range);

			virtual bool OnInitialized() override;
			virtual bool OnActivated() override;
			virtual void OnUpdated() override;
			virtual bool OnInactivated() override;
			virtual void OnFinalized() noexcept override;
		private:
			/*�w��n�`���W�Œn�`���W�n��]�x�̃O���[�o�����W�n��]�x�Ƃ̍����擾����*/
			virtual double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2D<double>& ground_pos)const = 0;
		};
	}
}
