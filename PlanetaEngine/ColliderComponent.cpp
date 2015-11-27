#include "ColliderComponent.h"
#include "IGameObjectAccessor.h"
#include "CollisionDetectProcess.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "TransformComponent.h"
#include "Matrix.h"

namespace planeta_engine {
	namespace components {
		bool ColliderComponent::Initialize_()
		{
			_collision_detect_process = scene().game_process_manager().GetSystemProcess<system_processes::CollisionDetectProcess>();
			if (_collision_detect_process) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("�Փ˔���v���Z�X���擾�ł��܂���ł����B", __FUNCTION__);
				return false;
			}
		}

		bool ColliderComponent::Activated_()
		{
			_ResistToCollisionDetectProcess();
			return true;
		}

		bool ColliderComponent::InActivated_()
		{
			_RemoveFromCollisionDetectProcess();
			return true;
		}

		void ColliderComponent::_ResistToCollisionDetectProcess()
		{
			if (_collision_detect_process) {
				if (collision_group_name_.length() == 0) {
					debug::SystemLog::instance().LogError("�Փ˃O���[�v���ݒ肳��Ă��܂���B", __FUNCTION__);
				} else {
					_collision_detect_process->Resist(std::static_pointer_cast<ColliderComponent>(this_shared()));
				}
			}
			else {
				debug::SystemLog::instance().LogError("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
			}
		}

		void ColliderComponent::_RemoveFromCollisionDetectProcess()
		{
			if (_collision_detect_process) {
				_collision_detect_process->Remove(this);
			}
			else {
				debug::SystemLog::instance().LogError("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
			}
		}

		void ColliderComponent::Finalize_()noexcept
		{

		}

		const Vector2D<double> ColliderComponent::GetCollisionCenterPosition() const
		{
			const TransformComponent& transform = game_object().transform();
			Vector2D<double> relation_position = math::RotationalTransformation(transform.global_rotation_rad(), position_); //�Q�[���I�u�W�F�N�g����̑��Έʒu
			relation_position.x *= transform.global_scale().x; //�������g��𔽉f
			relation_position.y *= transform.global_scale().y; //�c�����g��𔽉f
			return transform.global_position() + relation_position;
		}

		const double ColliderComponent::GetCollisionScale() const
		{
			const TransformComponent& transform = game_object().transform();
			return transform.global_scale().x > transform.global_scale().y ? transform.global_scale().y : transform.global_scale().x;
		}

		const double ColliderComponent::GetCollisionRotationRad() const
		{
			return game_object().transform().global_rotation_rad() + rotation_rad_;
		}

		void ColliderComponent::collision_group(const std::string& cg) {
			if (_collision_detect_process) { //�Փ˔���v���Z�X���擾����Ă�����Փ˃O���[�vID��ύX����
				if (_collision_detect_process->ChangeCollisionGroup(this, cg)) {
					collision_group_name_ = cg;
				} else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�Փ˃O���[�v��", collision_group_name_, "����",cg,"�ɕύX�ł��܂���ł����B");
				}
			} else {
				collision_group_name_ = cg;
			}
		}

		void ColliderComponent::collide_with_ground_flag(bool flag) {
			if (_collision_detect_process) { //�Փ˔���v���Z�X���擾����Ă�����t���O��ύX����
				if (_collision_detect_process->ChangeCollisionWithGroundFlag(this,flag)) {
					collide_with_ground_flag_ = flag;
				} else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�n�`�Ƃ̏Փ˃t���O��", collide_with_ground_flag_ ? "true" : "false", "����", flag ? "true" : "false", "�ɕύX�ł��܂���ł����B");
				}
			} else {
				collide_with_ground_flag_ = flag;
			}
		}

	}
}