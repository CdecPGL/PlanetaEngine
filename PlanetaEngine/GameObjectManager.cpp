#include "GameObjectManager.h"
#include "GameObject.h"

namespace PlanetaEngine{
	namespace Game{
		
		GameObjectManager::~GameObjectManager()
		{

		}

		bool GameObjectManager::Process()
		{
			return true;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go)
		{
			if (go->ID() > -1){ return go->ID(); } //すでにIDを持っている(マネージャーに登録されているものはIDを返すだけ)
			_game_objects.insert(std::make_pair(_id_counter, go));
			return _id_counter++;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go, const std::string& name)
		{
			int id = Resist(go);
			_name_id_map.emplace(name, id);
			return id;
		}

		void GameObjectManager::TakeOver(const GameObjectManager& gom)
		{
			for (auto& go : gom._game_objects){
				_game_objects.insert(std::move(go));
			}
		}

	}
}