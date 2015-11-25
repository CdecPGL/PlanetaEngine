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
				if (collision_group_name_.length() > 0) { //何らかのグループ名が設定されていたらIDの取得を試みる
					collision_group_id_ = _collision_detect_process->collision_group_matrix().GetCollisionGroupID(collision_group_name_);
					if (collision_group_id_ < 0) { debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "衝突グループ", collision_group_name_, "のIDを取得できませんでした。"); }
				}
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("衝突判定プロセスを取得できませんでした。", __FUNCTION__);
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
				debug::SystemLog::instance().LogWarning("衝突判定プロセスが取得できていません。", __FUNCTION__);
			}
		}

		void ColliderComponent::_RemoveFromCollisionDetectProcess()
		{
			if (_collision_detect_process) {
				_collision_detect_process->Remove(std::static_pointer_cast<ColliderComponent>(this_shared()));
			}
			else {
				debug::SystemLog::instance().LogWarning("衝突判定プロセスが取得できていません。", __FUNCTION__);
			}
		}

		void ColliderComponent::Finalize_()noexcept
		{

		}

		const Vector2D<double> ColliderComponent::GetCollisionCenterPosition() const
		{
			const TransformComponent& transform = game_object().transform();
			Vector2D<double> relation_position = math::RotationalTransformation(transform.global_rotation_rad(), position_); //ゲームオブジェクトからの相対位置
			relation_position.x *= transform.global_scale().x; //横方向拡大を反映
			relation_position.y *= transform.global_scale().y; //縦方向拡大を反映
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
			if (_collision_detect_process) { //衝突判定プロセスが取得されていたら衝突グループIDの取得を試みる
				collision_group_id_ = _collision_detect_process->collision_group_matrix().GetCollisionGroupID(collision_group_name_);
				if (collision_group_id_ < 0) { debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "衝突グループ", collision_group_name_, "のIDを取得できませんでした。"); }
			}
		}

	}
}