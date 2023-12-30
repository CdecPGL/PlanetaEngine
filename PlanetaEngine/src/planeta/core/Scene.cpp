#include "Scene.hpp"
//Modules
#include "game_object_manager.hpp"
#include "TaskManager.hpp"
#include "collision_world.hpp"
#include "draw_system.hpp"
#include "TransformSystem.hpp"

#include "log_utility.hpp"
#include "ScreenDrawer2D.hpp"
#include "ScreenDrawerGUI.hpp"

namespace plnt::private_ {
	scene::scene() = default;
	scene::~scene() = default;

	bool scene::initialize() const {
		//モジュールを初期化
		if (iterate_scene_module([](scene_module &sm) { return sm.initialize(); })) { return true; }
		PE_LOG_ERROR("Sceneの初期化に失敗しました。");
		return false;
	}

	bool scene::finalize() const {
		//モジュールの終了処理を行う
		reverse_iterate_scene_module([](scene_module &sm) {
			sm.finalize();
			return true;
		});
		return true;
	}

	void scene::update() const {
		try {
			task_manager_->ExcuteTask(); //タスク実行
		} catch (null_weak_pointer_exception &e) {
			PE_LOG_ERROR("TaskManager::Updateで無効なWeakPointerが参照されました。", e.what());
			return;
		}
		try {
			//各シーンモジュールの更新
			iterate_scene_module([](scene_module &sm) {
				sm.update();
				return true;
			});
		} catch (null_weak_pointer_exception &e) {
			PE_LOG_ERROR("シーンモジュールの更新において無効なWeakPointerが参照されました。", e.what());
		}
	}

	bool scene::iterate_scene_module(std::function<bool(scene_module &)> &&proc) const {
		if (!proc(*game_object_manager_)) { return false; }
		if (!proc(*collision_world_)) { return false; }
		if (!proc(*draw_system_)) { return false; }
		if (!proc(*transform_system_)) { return false; }
		if (!proc(*task_manager_)) { return false; }
		return true;
	}

	bool scene::reverse_iterate_scene_module(std::function<bool(scene_module &)> &&proc) const {
		if (!proc(*task_manager_)) { return false; }
		if (!proc(*transform_system_)) { return false; }
		if (!proc(*draw_system_)) { return false; }
		if (!proc(*collision_world_)) { return false; }
		if (!proc(*game_object_manager_)) { return false; }
		return true;
	}

	void scene::debug_information_add_handle(i_debug_information_adder &di_adder) const {
		iterate_scene_module([&di_adder](scene_module &sm) {
			sm.debug_information_add_handle(di_adder);
			return true;
		});
	}

	WeakPointer<collision_world> scene::collision_world_internal_pointer() {
		return collision_world_;
	}

	WeakPointer<draw_system> scene::draw_system_internal_pointer() { return draw_system_; }

	WeakPointer<game_object_manager> scene::game_object_manager_internal_pointer() {
		return game_object_manager_;
	}

	WeakPointer<TaskManager> scene::task_manager_internal_pointer() { return task_manager_; }

	WeakPointer<TransformSystem> scene::transform_system_internal_pointer() {
		return transform_system_;
	}

	i_collision_world &scene::collision_world() { return *collision_world_; }

	i_draw_system &scene::draw_system() { return *draw_system_; }

	i_game_object_manager &scene::game_object_manager() { return *game_object_manager_; }

	i_task_manager &scene::task_manager() { return *task_manager_; }

	i_transform_system &scene::transform_system() { return *transform_system_; }

	void scene::set_task_manager(std::shared_ptr<TaskManager> &&mgr) { task_manager_ = std::move(mgr); }

	void scene::set_game_object_manager(std::shared_ptr<private_::game_object_manager> &&mgr) {
		game_object_manager_ = std::move(mgr);
	}

	void scene::set_collision_world(std::shared_ptr<private_::collision_world> &&mgr) {
		collision_world_ = std::move(mgr);
	}

	void scene::set_draw_system(std::shared_ptr<private_::draw_system> &&mgr) { draw_system_ = std::move(mgr); }

	void scene::set_transform_manager(std::shared_ptr<TransformSystem> &&mgr) {
		transform_system_ = std::move(mgr);
	}

	void scene::set_scene_to_modules() {
		iterate_scene_module([this](scene_module &sm) {
			sm.set_scene(shared_from_this());
			return true;
		});
	}
}
