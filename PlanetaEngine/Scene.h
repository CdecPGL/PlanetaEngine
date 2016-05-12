#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "Object.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class GameObjectManager;
	class TaskManager;
	namespace core{
		class SceneSystemSetUpper;
		class IGameAccessor;
		struct SceneData;
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
		class SceneModule;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,private utility::NonCopyable<Scene>{
			friend SceneSystemSetUpper;
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
			std::unique_ptr<TaskManager> task_manager_; //ゲームプロセスマネージャ
			std::unique_ptr<GameObjectManager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::unique_ptr<CollisionWorld> collision_world_; //コリジョンワールド
			std::unique_ptr<GameObjectDrawSystem> gameobject_draw_system_; //ゲームオブジェクト描画システム
			std::unique_ptr<TransformSystem> transform_system_; //トランスフォームシステム
			bool IterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する
			bool ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する
			std::list<SceneModule*> scene_module_list_;

			std::shared_ptr<SceneData> scene_data_; //シーンデータ
		};
	}
}