#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Object.h"
#include "SceneAccessorForSetUp.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

#include "Camera.h"

namespace planeta_engine{
	class GameObjectManager;
	class TaskManager;
	namespace core{
		class IGameAccessor;
		class Screen;
		struct SceneData;
		class CollisionWorld;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,private utility::NonCopyable<Scene>{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*シーンの初期化*/
			bool Initialize();
			/*シーンの終了処理*/
			bool Finalize();
			/*シーンの更新*/
			void Update();

			/*ゲームクラスへのアクセス*/
			IGameAccessor& game_accessor() { return game_; }

			/*設定関数*/
			SceneData& RefSceneData() { return *scene_data_; }

			/*初期化用関数*/
			void PrepareSceneData();
			void RegisterSceneDataToModules();

		private:
			IGameAccessor& game_;
			std::unique_ptr<TaskManager> game_process_manager_; //ゲームプロセスマネージャ
			std::unique_ptr<GameObjectManager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::unique_ptr<CollisionWorld> collision_world_; //コリジョンワールド
			bool ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する

			std::unique_ptr<Camera> camera_;

			std::shared_ptr<SceneData> scene_data_; //シーンデータ
		};
	}
}