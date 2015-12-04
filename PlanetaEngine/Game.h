#pragma once

#include <memory>
#include "Object.h"
#include "IGameAccessor.h"
#include "IGameSetUp.h"
#include "SceneManager.h"
#include "KeyInputManager.h"
#include "ResourceManager.h"


namespace planeta_engine {
	namespace core {
		class Game: public Object
		,public core::IGameAccessor,public core::IGameSetUp{
		public:
			Game();
			~Game();
			/*システム関数*/
			bool Initialize();
			void Finalize();
			/*エンジンのステータス*/
			enum class Status { Continue, Quit, Error };
			Status Update();			
		protected:
			IGameSetUp& game_setup_access() { return *this; }
		private:
			Game(const Game&) = delete;
			Game& operator=(const Game&) = delete;
			std::unique_ptr<SceneManager> _scene_manager;
			std::unique_ptr<KeyInputManager> _key_input_manager;
			std::shared_ptr<core::CollisionGroupMatrix> collision_group_matrix_; //衝突マトリックス
			/*システムの初期化と終了処理*/
			bool InitializeSystem();
			void FinalizeSystem();
			/*ゲーム固有の初期化と終了処理*/
			virtual bool InitializeGame() = 0;
			virtual void FinalizeGame() = 0;

			/*初期化用関数群*/
			SceneManager& scene_manager()override { return *_scene_manager; }
			ResourceManager& resource_manager()override { return ResourceManager::instance(); }
			KeyInputManager& key_input_manager()override { return *_key_input_manager; }
			core::CollisionGroupMatrix& RefCollisionGroupMatrix()override { return *collision_group_matrix_; }
			std::shared_ptr<const core::CollisionGroupMatrix> GetCollisionGroupMatrix()const override { return collision_group_matrix_; }
		};
	}
}
