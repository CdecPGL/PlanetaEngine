#pragma once

#include <memory>
#include <string>
#include "Object.h"
#include "ISceneSetup.h"
#include "ISceneAccessForGameObject.h"
#include "ISceneAccessForGameProcess.h"
#include "ISceneAccessForUI.h"
#include "WeakPointer.h"
#include "SharedPointerInstance.h"

#include "GameObjectManager.h"
#include "GameProcessManager.h"
#include "UIManager.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class Scene : public Object,public utility::SharedPointerInstance<Scene>
			,public ISceneSetup,public ISceneAccessForGameObject,public ISceneAccessForGameProcess,public ISceneAccessForUI{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*マネージャのポインターをセット*/
			void SetManagerPointer();
			/*シーンの初期化*/
			bool Initialize();
			/*シーンの終了処理*/
			bool Finalize();
			/*シーンの更新*/
			void Update();
			/*プロセスマネージャのインスタンスを取得*/
			game::GameProcessManager& game_process_manager()override { return *game_process_manager_; }
			/*ゲームオブジェクトマネージャのインスタンスを取得*/
			game::GameObjectManager& game_object_manager()override { return *game_object_manager_; }
			/*UIマネージャのインスタンスを取得*/
			game::UIManager& ui_manager()override { return *ui_manager_; }
			/*ゲームクラスへのアクセス*/
			IGameAccessor& game_accessor() { return game_; }
		private:
			Scene(const Scene&) = delete;
			Scene(Scene&&) = delete;
			Scene& operator=(const Scene&) = delete;
			Scene& operator=(Scene&&) = delete;

			IGameAccessor& game_;
			std::unique_ptr<game::GameProcessManager> game_process_manager_;
			std::unique_ptr<game::GameObjectManager> game_object_manager_;
			std::unique_ptr<game::UIManager> ui_manager_;


		};
	}
}