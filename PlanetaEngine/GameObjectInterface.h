#pragma once

namespace planeta_engine {
	class IGameObject;
	class BaseGOI {
	public:
		BaseGOI(IGameObject& p_game_object) :game_object(p_game_object) {};
		IGameObject& game_object;
	};

	class GameObjectInterface :public virtual BaseGOI {

	};
}