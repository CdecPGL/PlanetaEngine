#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

namespace planeta_engine {
	template<const char* GameObjectID, class GameObjectType>
	class GameObjectRegisterer;
	class GameObjectManager;
	class GameObjectBase;
	class GameObjectFactory {
		template<const char* GameObjectID, class GameObjectType>
		friend class GameObjectRegisterer<GameObjectID, GameObjectType>;
		friend class GameOBjectManager;
	private:
		GameObjectFactory();
		static GameObjectFactory& GetInstance();
		void RegisterGameObject(const std::string& id);
		std::shared_ptr<GameObjectBase> CreateGameObject(const std::string& id);

		std::unordered_map<std::string, std::function<std::shared_ptr<GameObjectBase>()>> creator_map_;
	};
}
