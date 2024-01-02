#pragma once

#include <memory>
#include <functional>

#include "object.hpp"
#include "i_scene_internal.hpp"
#include "WeakPointer.hpp"
#include "non_copyable.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		class scene_module;

		// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		class scene final : public object, public i_scene_internal, public std::enable_shared_from_this<scene>
		                    , util::non_copyable<scene> {
		public:
			scene();
			scene(const scene &name) = delete;
			scene(scene &&name) = delete;
			~scene() override;
			scene &operator=(scene &&name) = delete;

			/*シーンの初期化*/
			bool initialize() const;
			/*シーンの終了処理*/
			bool finalize() const;
			/*シーンの更新*/
			void update() const;
			/*Moduleにシーンをセット*/
			void set_scene_to_modules();
			/*シーンモジュール設定関数(Initialize、SetSceneToModules呼び出し前に実行する必要がある)*/
			void set_task_manager(std::shared_ptr<private_::task_manager> &&mgr);
			void set_game_object_manager(std::shared_ptr<private_::game_object_manager> &&mgr);
			void set_collision_world(std::shared_ptr<private_::collision_world> &&mgr);
			void set_draw_system(std::shared_ptr<private_::draw_system> &&mgr);
			void set_transform_manager(std::shared_ptr<private_::transform_system> &&mgr);
			/*シーンモジュールの内部アクセスポインタ取得関数*/
			[[nodiscard]] weak_pointer<private_::collision_world> collision_world_internal_pointer() override;
			[[nodiscard]] weak_pointer<private_::draw_system> draw_system_internal_pointer() override;
			[[nodiscard]] weak_pointer<private_::game_object_manager> game_object_manager_internal_pointer() override;
			[[nodiscard]] weak_pointer<private_::task_manager> task_manager_internal_pointer() override;
			[[nodiscard]] weak_pointer<private_::transform_system> transform_system_internal_pointer() override;
			/*シーンモジュールインターフェイスアクセス関数*/
			[[nodiscard]] i_collision_world &collision_world() override;
			[[nodiscard]] i_draw_system &draw_system() override;
			[[nodiscard]] i_game_object_manager &game_object_manager() override;
			[[nodiscard]] i_task_manager &task_manager() override;
			[[nodiscard]] i_transform_system &transform_system() override;
			/*その他関数*/
			void debug_information_add_handle(i_debug_information_adder &di_adder) const;

		private:
			std::shared_ptr<private_::task_manager> task_manager_; //ゲームプロセスマネージャ
			std::shared_ptr<private_::game_object_manager> game_object_manager_; //ゲームオブジェクトマネージャ
			std::shared_ptr<private_::collision_world> collision_world_; //コリジョンワールド
			std::shared_ptr<private_::draw_system> draw_system_; //ゲームオブジェクト描画システム
			std::shared_ptr<private_::transform_system> transform_system_; //トランスフォームシステム
			bool iterate_scene_module(std::function<bool(scene_module &)> &&proc) const; //シーンモジュールに操作を適用する
			bool reverse_iterate_scene_module(std::function<bool(scene_module &)> &&proc) const; //シーンモジュールに操作を適用する
		};
	}
}
