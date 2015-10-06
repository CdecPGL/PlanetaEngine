#pragma once

#include <memory>
#include "Object.h"
#include "IGameAccessor.h"
#include "IGameSetUp.h"
#include "SceneManager.h"
#include "KeyInputManager.h"
#include "ResourceManager.h"
#include "TwoKeyMap.h"


namespace planeta_engine {
	namespace core {
		class Game final: public Object
		,public core::IGameAccessor,public core::IGameSetUp{
		public:
			Game();
			~Game() = default;
			/*システム関数*/
			bool Initialize();
			void Finalize();
			/*エンジンのステータス*/
			enum class Status { Continue, Quit, Error };
			Status Update();
			/*初期化用関数群*/
			SceneManager& scene_manager()override { return *_scene_manager; }
			ResourceManager& resource_manager()override { return ResourceManager::instance(); }
			KeyInputManager& key_input_manager()override { return *_key_input_manager; }
			void SetCollisionMap(const utility::TwoKeyHashMap<std::string, bool>& col_map)override { collision_map_ = col_map; }
			void SetCollisionMap(utility::TwoKeyHashMap<std::string, bool>&& col_map)override { collision_map_ = std::move(col_map); }
			const utility::TwoKeyHashMap<std::string, bool>& GetCollisionMap()const override{ return collision_map_; }
		private:
			std::unique_ptr<SceneManager> _scene_manager;
			std::unique_ptr<KeyInputManager> _key_input_manager;
			utility::TwoKeyHashMap<std::string, bool> collision_map_; //衝突マップ
		};
	}
}
