#pragma once

#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class IDrawSystem {
	public:
		virtual ~IDrawSystem() = 0 {};
		/*! スクリーン座標をGameObject座空間標に変換*/
		virtual Vector2Dd CovertPositionScreenSpaceToGameObjectSpace(const Vector2Di& gui_space_pos)const = 0;
		/*! GameObject座標をスクリーン座標に変換*/
		virtual Vector2Di CovertPositionGameObjectSpaceToScreenSpace(const Vector2Dd& gameobject_space_pos)const = 0;
	};
}
