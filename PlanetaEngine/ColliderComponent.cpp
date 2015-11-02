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
				debug::SystemLog::instance().LogError("衝突判定プロセスを取得できませんでした。", "ColliderComponent::Initialize_");
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
				_ResistToCollisionDetectProcessByType(*_collision_detect_process);
			}
			else {
				debug::SystemLog::instance().LogWarning("衝突判定プロセスが取得できていません。", "ColliderComponent::_ResistToCollisionDetectProcess");
			}
		}

		void ColliderComponent::_RemoveFromCollisionDetectProcess()
		{
			if (_collision_detect_process) {
				_RemoveFromCollisionDetectProcessByType(*_collision_detect_process);
			}
			else {
				debug::SystemLog::instance().LogWarning("衝突判定プロセスが取得できていません。", "ColliderComponent::_RemoveFromCollisionDetectProcess");
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

	}
}