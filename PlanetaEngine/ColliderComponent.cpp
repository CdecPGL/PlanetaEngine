#include "ColliderComponent.h"
#include "IGameObjectAccessor.h"
#include "CollisionDetectProcess.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "TransformComponent.h"
#include "Matrix.h"
#include "CollisionGroupMatrix.h"

namespace planeta_engine {
	namespace components {
		bool ColliderComponent::Initialize_()
		{
			_collision_detect_process = scene().game_process_manager().GetSystemProcess<system_processes::CollisionDetectProcess>();
			if (_collision_detect_process) {
				if (collision_group_name_.length() > 0) { //���炩�̃O���[�v�����ݒ肳��Ă�����ID�̎擾�����݂�
					collision_group_id_ = _collision_detect_process->collision_group_matrix().GetCollisionGroupID(collision_group_name_);
					if (collision_group_id_ < 0) { debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "�Փ˃O���[�v", collision_group_name_, "��ID���擾�ł��܂���ł����B"); }
				}
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
				_collision_detect_process->Resist(std::static_pointer_cast<ColliderComponent>(this_shared()));
			}
			else {
				debug::SystemLog::instance().LogWarning("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
			}
		}

		void ColliderComponent::_RemoveFromCollisionDetectProcess()
		{
			if (_collision_detect_process) {
				_collision_detect_process->Remove(std::static_pointer_cast<ColliderComponent>(this_shared()));
			}
			else {
				debug::SystemLog::instance().LogWarning("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
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
			collision_group_name_ = cg;
			if (_collision_detect_process) { //�Փ˔���v���Z�X���擾����Ă�����Փ˃O���[�vID�̎擾�����݂�
				collision_group_id_ = _collision_detect_process->collision_group_matrix().GetCollisionGroupID(collision_group_name_);
				if (collision_group_id_ < 0) { debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "�Փ˃O���[�v", collision_group_name_, "��ID���擾�ł��܂���ł����B"); }
			}
		}

	}
}