#pragma once

#include <type_traits>
#include "GameObjectBase.h"
#include "GameObjectInterface.h"

namespace planeta_engine {
	//ゲームオブジェクトを定義するときは、このクラスを継承する。テンプレート引数にはGameObjectInterfaceを任意個指定する。
	template<typename... GOI>
	class GameObject : public GameObjectBase , public GOI... {
		static_assert(std::is_base_of<GameObjectInterface, GOI>::value == false, "GOI must derive GameObjectInterface.")...;
	};
}
