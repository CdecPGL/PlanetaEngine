#pragma once

#include "i_game_object_manager.hpp"
#include "scene_module.hpp"

namespace plnt::private_ {
	class game_object_manager : public i_game_object_manager, public scene_module {
	public:
		game_object_manager() = default;
		game_object_manager(const game_object_manager &) = delete;
		game_object_manager &operator=(const game_object_manager &) = delete;
		// NOLINTNEXTLINE(modernize-use-equals-default, clang-diagnostic-microsoft-pure-definition)
		~game_object_manager() override = 0 { }
		game_object_manager &operator=(game_object_manager &&) = delete;
		game_object_manager(game_object_manager &&) = delete;

		/*ゲームオブジェクト登録解除(終了処理を行う)*/
		virtual bool remove_game_object(int id) = 0;
		/*有効化*/
		virtual bool activate_game_object(int id) = 0;
		/*無効化*/
		virtual bool inactivate_game_object(int id) = 0;
		/*すべてのゲームオブジェクトを破棄*/
		virtual void remove_all_game_objects() = 0;
	};
}
