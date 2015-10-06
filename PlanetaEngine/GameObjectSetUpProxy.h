#pragma once

#include "GameObject.h"

namespace planeta_engine{
	namespace game{
		class GameObject;
		class GameObjectSetUpProxy{
		public:
			GameObjectSetUpProxy(GameObject& go) :_game_object(go){};
			~GameObjectSetUpProxy(){};
			template<class C>
			utility::WeakPointer<C> AddComponent(){return  _game_object._add_component<C>(); }
		private:
			GameObject& _game_object;
		};
	}
}