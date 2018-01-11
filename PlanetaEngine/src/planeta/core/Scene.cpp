#include "planeta/core/Scene.hpp"
//Modules
#include "planeta/core/GameObjectManager.hpp"
#include "TaskManager.hpp"
#include "planeta/core/CollisionWorld.hpp"
#include "planeta/core/DrawSystem.hpp"
#include "planeta/core/TransformSystem.hpp"

#include "planeta/core/LogUtility.hpp"
#include "planeta/core/ScreenDrawer2D.hpp"
#include "planeta/core/ScreenDrawerGUI.hpp"

namespace plnt{
	namespace private_{

		Scene::Scene()
		{
			
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//モジュールを初期化
			if (IterateSceneModule_([](private_::SceneModule& sm) {return sm.Initialize(); })) {
				return true;
			}
			else {
				PE_LOG_ERROR("Sceneの初期化に失敗しました。");
				return false;
			}
		}

		bool Scene::Finalize()
		{
			//モジュールの終了処理を行う
			ReverseIterateSceneModule_([](SceneModule& sm) {sm.Finalize(); return true; });
			return true;
		}

		void Scene::Update()
		{
			try {
				task_manager_->ExcuteTask(); //タスク実行
			}
			catch (NullWeakPointerException& e) {
				PE_LOG_ERROR("TaskManager::Updateで無効なWeakPointerが参照されました。", e.what());
				return;
			}try {
				//各シーンモジュールの更新
				IterateSceneModule_([](SceneModule& sm) {sm.Update(); return true; });
			}
			catch (NullWeakPointerException& e) {
				PE_LOG_ERROR("シーンモジュールの更新において無効なWeakPointerが参照されました。", e.what());
				return;
			}
		}

		bool Scene::IterateSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			if (!proc(*game_object_manager_)) { return false; }
			if (!proc(*collision_world_)) { return false; }
			if (!proc(*draw_system_)) { return false; }
			if (!proc(*transform_system_)) { return false; }
			if (!proc(*task_manager_)) { return false; }
			return true;
		}

		bool Scene::ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			if (!proc(*task_manager_)) { return false; }
			if (!proc(*transform_system_)) { return false; }
			if (!proc(*draw_system_)) { return false; }
			if (!proc(*collision_world_)) { return false; }
			if (!proc(*game_object_manager_)) { return false; }
			return true;
		}

		void Scene::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			IterateSceneModule_([&di_adder](SceneModule& sm) {sm.DebugInformationAddHandle(di_adder); return true; });
		}

		plnt::WeakPointer<plnt::private_::CollisionWorld> Scene::collision_world_internal_pointer() {
			return collision_world_;
		}

		plnt::WeakPointer<DrawSystem> Scene::draw_system_internal_pointer() {
			return draw_system_;
		}

		plnt::WeakPointer<plnt::private_::GameObjectManager> Scene::game_object_manager_internal_pointer() {
			return game_object_manager_;
		}

		plnt::WeakPointer<plnt::private_::TaskManager> Scene::task_manager_internal_pointer() {
			return task_manager_;
		}

		plnt::WeakPointer<plnt::private_::TransformSystem> Scene::transform_system_internal_pointer() {
			return transform_system_;
		}

		plnt::ICollisionWorld& Scene::collision_world() {
			return *collision_world_;
		}

		plnt::IDrawSystem& Scene::draw_system() {
			return *draw_system_;
		}

		plnt::IGameObjectManager& Scene::game_object_manager() {
			return *game_object_manager_;
		}

		plnt::ITaskManager& Scene::task_manager() {
			return *task_manager_;
		}

		plnt::ITransformSystem& Scene::transform_system() {
			return *transform_system_;
		}

		void Scene::SetTaskManager(std::shared_ptr<TaskManager>&& mgr) {
			task_manager_ = std::move(mgr);
		}

		void Scene::SetGameObjectManager(std::shared_ptr<GameObjectManager>&& mgr) {
			game_object_manager_ = std::move(mgr);
		}

		void Scene::SetCollisionWorld(std::shared_ptr<CollisionWorld>&& mgr) {
			collision_world_ = std::move(mgr);
		}

		void Scene::SetDrawSystem(std::shared_ptr<DrawSystem>&& mgr) {
			draw_system_ = std::move(mgr);
		}

		void Scene::SetTransformManager(std::shared_ptr<TransformSystem>&& mgr) {
			transform_system_ = std::move(mgr);
		}

		void Scene::SetSceneToModules() {
			IterateSceneModule_([this](SceneModule& sm) {sm.SetScene(shared_from_this()); return true; });
		}

	}
}