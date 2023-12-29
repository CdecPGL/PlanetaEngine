#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "Object.hpp"
#include "i_scene_internal.hpp"
#include "WeakPointer.hpp"
#include "NonCopyable.hpp"
#include "NonOwingPointer.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		class SceneModule;

		class Scene final : public Object, public i_scene_internal, public std::enable_shared_from_this<Scene>
		                    , private util::NonCopyable<Scene> {
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
			void SetTaskManager(std::shared_ptr<TaskManager> &&mgr);
			void SetGameObjectManager(std::shared_ptr<private_::game_object_manager> &&mgr);
			void SetCollisionWorld(std::shared_ptr<private_::collision_world> &&mgr);
			void SetDrawSystem(std::shared_ptr<private_::draw_system> &&mgr);
			void SetTransformManager(std::shared_ptr<TransformSystem> &&mgr);
			/*シーンモジュールの内部アクセスポインタ取得関数*/
			WeakPointer<private_::collision_world> collision_world_internal_pointer() override;
			WeakPointer<private_::draw_system> draw_system_internal_pointer() override;
			WeakPointer<private_::game_object_manager> game_object_manager_internal_pointer() override;
			WeakPointer<TaskManager> task_manager_internal_pointer() override;
			WeakPointer<TransformSystem> transform_system_internal_pointer() override;
			/*シーンモジュールインターフェイスアクセス関数*/
			i_collision_world &collision_world() override;
			i_draw_system &draw_system() override;
			i_game_object_manager &game_object_manager() override;
			i_task_manager &task_manager() override;
			i_transform_system &transform_system() override;
			/*その他関数*/
			void DebugInformationAddHandle(i_debug_information_adder &di_adder);

		private:
			std::shared_ptr<TaskManager> task_manager_; //ゲームプロセスマネージャ
			std::shared_ptr<private_::game_object_manager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::shared_ptr<private_::collision_world> collision_world_; //コリジョンワールド
			std::shared_ptr<private_::draw_system> draw_system_; //ゲームオブジェクト描画システム
			std::shared_ptr<TransformSystem> transform_system_; //トランスフォームシステム
			bool IterateSceneModule_(std::function<bool(SceneModule &)> &&proc); //シーンモジュールに操作を適用する
			bool ReverseIterateSceneModule_(std::function<bool(SceneModule &)> &&proc); //シーンモジュールに操作を適用する
		};
	}
}
