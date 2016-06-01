#pragma once

#include <functional>

namespace planeta {
	class CCollider2D;
	class IGameObject;
	class CTransform2D;
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	namespace core {
		//���̃f�[�^��CollisionWorld�ŕێ�����Ă���Ԃ͂���Colider�͑��݂��Ă���͂��Ȃ̂ŁA�����o�ɎQ�Ƃ��܂�ł��悢�B
		struct Collider2DData {
			CCollider2D& collider2d;
			IGameObject& game_object;
			CTransform2D& transform2d;
			std::function<void(const EACollisionWithCollider2D&)> collide_with_collider_event_evoker;
			std::function<void(const EACollisionWithGround2D&)> collide_with_ground_event_evoker;
		};
	}
}