#include "DrawComponent.h"
#include "IGameObjectAccessor.h"
#include "GameObjectDrawProcess.h"
#include "TransformComponent.h"
#include "Matrix.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"

namespace planeta_engine {
	namespace components {

		DrawComponent::DrawComponent() :_draw_priority(0), _rotation_rad(0.0), _scale(1.0, 1.0)
		{

		}

		void DrawComponent::draw_priority(int priority)
		{
			_draw_priority = priority;
			//ゲームオブジェクトがアクティブなら優先度更新
			if (game_object().is_active()) {
				_UpdatePriority();
			}
		}

		math::Vector2Dd DrawComponent::GetDrawCenterPosition() const
		{
			const TransformComponent& transform = game_object().transform();
			math::Vector2Dd relation_position = math::RotationalTransformation(transform.global_rotation_rad(), _position); //ゲームオブジェクトからの相対位置
			relation_position.x *= transform.global_scale().x; //横方向拡大を反映
			relation_position.y *= transform.global_scale().y; //縦方向拡大を反映
			return transform.global_position() + relation_position;
		}

		double DrawComponent::GetDrawRotationRed() const
		{
			return game_object().transform().global_rotation_rad() + _rotation_rad;
		}

		math::Vector2Dd DrawComponent::GetDrawScale() const
		{
			return math::Vector2Dd(game_object().transform().global_scale().x * _scale.x, game_object().transform().global_scale().y * _scale.y);
		}

		void DrawComponent::_ResistToDrawProcess()
		{
			if (_game_object_draw_process) {
				_game_object_draw_process->Resist(std::static_pointer_cast<DrawComponent>(this_shared()), _draw_priority);
			}
			else {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクト描画プロセスが取得できていません。", "DrawComponent::_ResistToDrawProcess");
			}
		}

		void DrawComponent::_RemoveFromDrawProcess()
		{
			if (_game_object_draw_process) {
				_game_object_draw_process->Remove(std::static_pointer_cast<DrawComponent>(this_shared()));
			}
			else {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクト描画プロセスが取得できていません。", "DrawComponent::_RemoveFromDrawProcess");
			}
		}

		void DrawComponent::_UpdatePriority()
		{
			if (_game_object_draw_process) {
				_game_object_draw_process->ChangePriority(std::static_pointer_cast<DrawComponent>(this_shared()), _draw_priority);
			}
		}


		bool DrawComponent::Initialize_()
		{
			_game_object_draw_process = scene().game_process_manager().GetSystemProcess<system_processes::GameObjectDrawProcess>();
			if (_game_object_draw_process) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("ゲームオブジェクト描画プロセスを取得できませんでした。", "DrawComponent::Initialize_");
				return false;
			}
		}

		bool DrawComponent::Activated_()
		{
			_ResistToDrawProcess();
			return true;
		}

		bool DrawComponent::InActivated_()
		{
			_RemoveFromDrawProcess();
			return true;
		}
	}
}