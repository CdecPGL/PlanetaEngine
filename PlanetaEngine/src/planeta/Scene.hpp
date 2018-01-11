#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "planeta/core/Object.hpp"
#include "planeta/ISceneInternal.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "NonCopyable.hpp"
#include "planeta/core/NonOwingPointer.hpp"

namespace plnt{
	class IDebugInformationAdder;
	namespace private_{
		class SceneModule;
		class Scene final: public Object,public ISceneInternal ,public std::enable_shared_from_this<Scene>
			,private util::NonCopyable<Scene>{
		public:
			Scene();
			~Scene();
			/*シーンの初期化*/
			bool Initialize();
			/*シーンの終了処理*/
			bool Finalize();
			/*シーンの更新*/
			void Update();
			/*Moduleにシーンをセット*/
			void SetSceneToModules();
			/*シーンモジュール設定関数(Initialize、SetSceneToModules呼び出し前に実行する必要がある)*/
			void SetTaskManager(std::shared_ptr<TaskManager>&& mgr);
			void SetGameObjectManager(std::shared_ptr<GameObjectManager>&& mgr);
			void SetCollisionWorld(std::shared_ptr<CollisionWorld>&& mgr);
			void SetDrawSystem(std::shared_ptr<DrawSystem>&& mgr);
			void SetTransformManager(std::shared_ptr<TransformSystem>&& mgr);
			/*シーンモジュールの内部アクセスポインタ取得関数*/
			WeakPointer<CollisionWorld> collision_world_internal_pointer() override;
			WeakPointer<DrawSystem> draw_system_internal_pointer() override;
			WeakPointer<GameObjectManager> game_object_manager_internal_pointer() override;
			WeakPointer<TaskManager> task_manager_internal_pointer() override;
			WeakPointer<TransformSystem> transform_system_internal_pointer() override;
			/*シーンモジュールインターフェイスアクセス関数*/
			ICollisionWorld& collision_world() override;
			IDrawSystem& draw_system() override;
			IGameObjectManager& game_object_manager() override;
			ITaskManager& task_manager() override;
			ITransformSystem& transform_system() override;
			/*その他関数*/
			void DebugInformationAddHandle(IDebugInformationAdder& di_adder);
		private:
			std::shared_ptr<TaskManager> task_manager_; //ゲームプロセスマネージャ
			std::shared_ptr<GameObjectManager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::shared_ptr<CollisionWorld> collision_world_; //コリジョンワールド
			std::shared_ptr<DrawSystem> draw_system_; //ゲームオブジェクト描画システム
			std::shared_ptr<TransformSystem> transform_system_; //トランスフォームシステム
			bool IterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する
			bool ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //シーンモジュールに操作を適用する
		};
	}
}