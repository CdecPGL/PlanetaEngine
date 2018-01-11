#pragma once

#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class IDrawSystem {
	public:
		virtual ~IDrawSystem() = 0 {};
		/*! �X�N���[�����W��GameObject����ԕW�ɕϊ�*/
		virtual Vector2Dd CovertPositionScreenSpaceToGameObjectSpace(const Vector2Di& gui_space_pos)const = 0;
		/*! GameObject���W���X�N���[�����W�ɕϊ�*/
		virtual Vector2Di CovertPositionGameObjectSpaceToScreenSpace(const Vector2Dd& gameobject_space_pos)const = 0;
	};
}
