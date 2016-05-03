#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include "NonCopyable.h"

namespace planeta_engine {
	template<const char* GameObjectID, class GameObjectType>
	class GameObjectRegisterer;
	class GameObjectManager;
	class GameObjectBase;
	//ゲームオブジェクトファクトリ(シングルトンクラス)
	//GameObjectRegistererとGameObjectManagerのみが使用可能。
	class GameObjectFactory : private utility::NonCopyable<GameObjectFactory>{
		template<const char* GameObjectID, class GameObjectType>
		friend class GameObjectRegisterer;
		friend class GameObjectManager;
	private:
		GameObjectFactory();
		static GameObjectFactory& GetInstance();
		template<class GOT>
		void RegisterGameObject(const std::string& id) {
			static_assert(std::is_base_of<GameObjectBase, GOT>::value == true, "GOT must derive GameObjectBase.");
			creator_map_.emplace(id, []() {return GameObjectBase::Create<GOT>(); });
		}
		std::shared_ptr<GameObjectBase> CreateGameObject(const std::string& id);

		std::unordered_map<std::string, std::function<std::shared_ptr<GameObjectBase>()>> creator_map_;
	};
}
