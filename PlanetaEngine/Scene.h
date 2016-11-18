﻿#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "Object.h"
#include "IScene.h"
#include "WeakPointer.h"
#include "NonCopyable.h"
#include "NonOwingPointer.h"

namespace planeta{
	class TaskManager;
	class IDebugInformationAdder;
	namespace private_{
		class GameObjectManager;
		class SceneSystemSetUpper;
		struct SceneData;
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
		class SceneModule;
		class Scene : public Object,public IScene ,public std::enable_shared_from_this<Scene>
			,private util::NonCopyable<Scene>{
			friend SceneSystemSetUpper;
		public:
			Scene();
			~Scene();
			/*シーンの初期化*/
			bool Initialize();
			/*シーンの終了処理*/
			bool Finalize();
			/*シーンの更新*/
			void Update();

			/*設定関数*/
			SceneData& RefSceneData() { return *scene_data_; }

			/*初期化用関数*/
			void PrepareSceneData();
			void RegisterSceneDataToModules();

			void DebugInformationAddHandle(IDebugInformationAdder& di_adder);

		private:
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