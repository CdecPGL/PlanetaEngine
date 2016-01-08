#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Object.h"
#include "SceneAccessorForSetUp.h"
#include "ScenePublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

#include "GameObjectManager.h"
#include "GameProcessManager.h"
#include "GUIManager.h"
#include "Camera.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class Screen;
		struct SceneData;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,public ScenePublicInterface,private utility::NonCopyable<Scene>{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*シーンの初期化*/
			bool Initialize();
			/*シーンの終了処理*/
			bool Finalize();
			/*シーンの更新*/
			void Update();
			/*プロセスマネージャのインスタンスを取得*/
			game::GameProcessManager& game_process_manager()override { assert(game_process_manager_ != nullptr); return *game_process_manager_; }
			/*ゲームオブジェクトマネージャのインスタンスを取得*/
			game::GameObjectManager& game_object_manager()override { assert(game_object_manager_ != nullptr); return *game_object_manager_; }
			/*GUIマネージャのインスタンスを取得*/
			game::GUIManager& gui_manager()override { assert(ui_manager_ != nullptr); return *ui_manager_; }
			/*カメラへのアクセス*/
			Camera& camera()override { return *camera_; }
			/*ゲームクラスへのアクセス*/
			IGameAccessor& game_accessor() { return game_; }

			/*設定関数*/
			SceneData& RefSceneData() { return *scene_data_; }

			/*初期化用関数*/
			void RegisterSceneInterfaceToModules();
			void PrepareSceneData();
			void RegisterSceneDataToModules();
		private:
			IGameAccessor& game_;
			/*シーンモジュール*/
			std::unique_ptr<game::GameProcessManager> game_process_manager_; //ゲームプロセスマネージャ
			std::unique_ptr<game::GameObjectManager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::unique_ptr<game::GUIManager> ui_manager_; //GUIマネージャ
			bool ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する

			std::unique_ptr<Camera> camera_;

			std::unique_ptr<SceneData> scene_data_; //シーンデータ
		};
	}
}