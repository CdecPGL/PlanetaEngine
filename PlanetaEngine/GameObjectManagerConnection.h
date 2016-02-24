#pragma once

namespace planeta_engine {
	namespace core {
		struct SceneDataForGameObject;
	}
	namespace game {
		class GameObjectManager;
		class GameObjectManagerConnection final{
		public:
			GameObjectManagerConnection(GameObjectManager& gom,int id) :game_object_manager_(gom),id_(id) {}
			bool Activate();
			bool InActivate();
			void Dispose();
			core::SceneDataForGameObject& RefSceneData();
		private:
			int id_;
			bool connection_valid_ = true;
			GameObjectManager& game_object_manager_;
		};
	}
}
