#pragma once

namespace planeta_engine {
	namespace game {
		class GameObjectManager;
		class GameObjectResisterConnection final{
		public:
			GameObjectResisterConnection(GameObjectManager& gom,int id) :game_object_manager_(gom),id_(id) {}
			bool Activate();
			bool InActivate();
			void Dispose();
		private:
			int id_;
			GameObjectManager& game_object_manager_;
		};
	}
}
