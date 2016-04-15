#pragma once

#include "TransformCore.h"

namespace planeta_engine {
	class RootTransformCore final : public TransformCore {
	public:
		RootTransformCore();

		void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset)override;

		/*�A�N�Z�T*/
		const Vector2Dd& position()const override;
		void position(const Vector2Dd& pos)override;
		const Vector2Dd& scale()const override;
		void  scale(const Vector2Dd& s)override;
		const double rotation_rad()const override;
		void  rotation_rad(double rota_rad)override;

		const Vector2Dd& global_position()const override;
		void global_position(const Vector2Dd& pos)override;
		const double global_rotation_rad()const override;
		void  global_rotation_rad(double rota_rad)override;

		const Vector2Dd& velocity()const override;
		void  velocity(const Vector2Dd& vel)override;
		const double rotation_velocity_rad()const override;
		void  rotation_velocity_rad(double rota_vel_rad)override;

		const Vector2Dd& global_velocity()const override;
		void global_velocity(const Vector2Dd& vel)override;

		/*�n�`�֌W*/
		components::GroundComponent& ground()override;
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		utility::WeakPointer<components::GroundComponent> GetGround()const override;
		void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position)override;

		void SetParent(TransformCore* parent)override;
	private:
		void UpdateTransformDataGlobalByGround(); //�n�`���W����O���[�o�����W���Z�o
		void UpdateTransformDataGroundByGlobal(); //�n�`���W���烍�[�J�����W���Z�o
		void UpdatePhysicalDataGlobal(); //�n�`���W����O���[�o�����W���Z�o
		void UpdatePhysicalDataGround(); //�n�`���W���烍�[�J�����W���Z�o

		utility::WeakPointer<components::GroundComponent> ground_; //�R���X�g���N�^�Ń_�~�[���Z�b�g

		enum class CoordinateSystem { Global, Ground, None };
		
		TransformData ground_transform_data_;
		TransformData global_transform_data_;
		CoordinateSystem position_last_update = CoordinateSystem::None;
		CoordinateSystem rotation_last_update = CoordinateSystem::None;

		//�������
		Vector2Dd ground_velocity_; //�n�`���W�n�ł̑��x�B�����̓O���[�o�����W�n�ɏ�����
		Vector2Dd global_velocity_;
		double rotation_velocity_rad_ = 0.0;
		CoordinateSystem velocity_last_update = CoordinateSystem::None;


	};
}